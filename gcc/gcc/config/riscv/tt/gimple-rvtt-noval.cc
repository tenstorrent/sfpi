/* Pass to issue diagnostics for SFPU operations
   Copyright (C) 2026 Tenstorrent Inc.
   Originated Nathan Sidwell (nsidwell@tenstorrent.com, nathan@acm.org).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "rtl.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "gimple.h"
#include "tree-pass.h"
#include "ssa.h"
#include "gimple-iterator.h"
#include "gimple-pretty-print.h"
#include "tree-ssa.h"
#include "tree-ssa-propagate.h"
#include "tree-into-ssa.h"
#include "rvtt.h"

// We need to compute the v_if nesting regions to do the djob that the existing
// pass does.  This is a simple change to remove assign_lv's whose live input
// is novalue

static bool
lv_is_noval (tree var)
{
  // Should we look through PHIs?
  if (auto *insnd = rvtt_get_insn_data (SSA_NAME_DEF_STMT (var)))
    return insnd->id == rvtt_insn_data::sfpnovalue;

  return false;
}

static void
elide_assign_lv (gcall *assign_lv)
{
  if (dump_file)
    {
      fprintf (dump_file, "Removing noval-using ");
      print_gimple_stmt (dump_file, assign_lv, 2);
    }

  rvtt_substitute_value (gimple_call_lhs (assign_lv),
			 gimple_call_arg (assign_lv, 1));
  auto gsi = gsi_for_stmt (assign_lv);
  unlink_stmt_vdef (assign_lv);
  gsi_remove (&gsi, true);
}

namespace {

const pass_data pass_data_rvtt_noval_elide =
{
  GIMPLE_PASS, /* type */
  "rvtt_noval_elide", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_noval_elide : public gimple_opt_pass
{
public:
  pass_rvtt_noval_elide (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_noval_elide, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX;
  }
  virtual unsigned execute (function *fn) override
  {
    basic_block bb;
    std::vector<gcall *> assigns;

    FOR_EACH_BB_FN (bb, fn)
      for (gimple_stmt_iterator gsi = gsi_start_bb (bb);
	   !gsi_end_p (gsi); gsi_next (&gsi))
	{
	  if (auto *insnd = rvtt_get_insn_data (*gsi))
	    {
	      auto *call = as_a <gcall *> (*gsi);
	      if (insnd->id == rvtt_insn_data::sfpassign_lv
		  && gimple_call_lhs (call)
		  && lv_is_noval (gimple_call_arg (call, 0)))
		assigns.push_back (call);
	    }
	}

    for (auto *assign : assigns)
      elide_assign_lv (assign);

    return assigns.empty () ? 0 : TODO_update_ssa;
  }
};

} // anon namespace

gimple_opt_pass *
make_pass_rvtt_noval_elide (gcc::context *ctxt)
{
  return new pass_rvtt_noval_elide (ctxt);
}
