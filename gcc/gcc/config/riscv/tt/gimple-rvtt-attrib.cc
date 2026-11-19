/* Pass to restore lost memory space pointer attributes
   Copyright (C) 2022-2025 Tenstorrent Inc.
   Originated by Paul Keller (pkeller@tenstorrent.com).

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
#include <unordered_map>

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "gimple.h"
#include "cfghooks.h"
#include "tree-pass.h"
#include "ssa.h"
#include "cgraph.h"
#include "gimple-pretty-print.h"
#include "diagnostic-core.h"
#include "fold-const.h"
#include "trans-mem.h"
#include "stor-layout.h"
#include "print-tree.h"
#include "cfganal.h"
#include "tree-eh.h"
#include "gimple-iterator.h"
#include "gimple-fold.h"
#include "gimplify-me.h"
#include "gimple-walk.h"
#include "tree-cfg.h"
#include "tree-ssa-loop-manip.h"
#include "tree-ssa-loop-niter.h"
#include "tree-into-ssa.h"
#include "tree-dfa.h"
#include "tree-ssa.h"
#include "except.h"
#include "cfgloop.h"
#include "tree-ssa-propagate.h"
#include "value-prof.h"
#include "tree-inline.h"
#include "tree-ssa-live.h"
#include "omp-general.h"
#include "omp-expand.h"
#include "tree-cfgcleanup.h"
#include "gimplify.h"
#include "attribs.h"
#include "selftest.h"
#include "opts.h"
#include "asan.h"
#include "profile.h"
#include <vector>
#include <tuple>
#include "rvtt.h"

#if 0
#define DUMP(...) (void)fprintf (stderr, __VA_ARGS__)
#else
#define DUMP(...) (void)0
#endif

static tree process_node (std::unordered_map<gimple *, tree>& stmts, gimple *stmt);

static bool
decl_has_attrib (tree decl, const char *attrib)
{
  return decl && lookup_attribute (attrib, TYPE_ATTRIBUTES (TREE_TYPE (decl)));
}

static tree
check_node(std::unordered_map<gimple *, tree>& stmts, gimple *stmt, tree node)
{
  if (node != NULL_TREE &&
      TREE_CODE(node) == SSA_NAME)
    {
      if (decl_has_attrib(node, "rvtt_l1_ptr") ||
	  decl_has_attrib(node, "rvtt_reg_ptr"))
	{
	  return node;
	}
      return process_node(stmts, SSA_NAME_DEF_STMT(node));
    }

  return NULL_TREE;
}

static void
reinsert(std::unordered_map<gimple *, tree>& stmts, gimple *stmt, tree node)
{
  auto cached = stmts.find(stmt);
  //  cached->second = node;
}

static tree
process_node(std::unordered_map<gimple *, tree>& stmts, gimple *stmt)
{
  if (stmt == nullptr)
    {
      return NULL_TREE;
    }
  else
    {
      auto cached = stmts.find(stmt);
      if (cached != stmts.end())
	{
	  return cached->second;
	}
      stmts.insert(std::pair<gimple *, tree>(stmt, NULL_TREE));
      if (stmt->code == GIMPLE_PHI)
	{
	  for (unsigned int i = 0; i < gimple_phi_num_args (stmt); i++)
	    {
	      tree rhs = gimple_phi_arg_def(stmt, i);
	      if (TREE_CODE(rhs) == SSA_NAME)
		{
		  gimple *origin = SSA_NAME_DEF_STMT(rhs);
		  tree ret;
		  if ((ret = process_node(stmts, origin)) != NULL_TREE)
		    {
		      reinsert(stmts, stmt, ret);
		      return ret;
		    }
		}
	    }

	  reinsert(stmts, stmt, NULL_TREE);
	  return NULL_TREE;
	}
      else if (stmt->code == GIMPLE_ASSIGN)
	{
	  tree ret;
	  if ((ret = check_node(stmts, stmt, gimple_assign_rhs1(stmt))) != NULL_TREE)
	    {
	      reinsert(stmts, stmt, ret);
	      return ret;
	    }
	  if ((ret = check_node(stmts, stmt, gimple_assign_rhs2(stmt))) != NULL_TREE)
	    {
	      reinsert(stmts, stmt, ret);
	      return ret;
	    }
	  ret = check_node(stmts, stmt, gimple_assign_rhs3(stmt));
	  reinsert(stmts, stmt, ret);
	  return ret;
	}
      else if (stmt->code == GIMPLE_CALL)
	{
	  // XXXX check fn signature?
	  reinsert(stmts, stmt, NULL_TREE);
	  return NULL_TREE;
	}
    }

  reinsert(stmts, stmt, NULL_TREE);
  return NULL_TREE;
}

// GS memory aribiter workaround relies on memory space attributes
// All targets perform better if memory accesses are tagged with the memory space attributes
// Unfortnately, some passes drop these attributes
// Walking gimple/tree structures at the end of the RTL passes looking for the
// attributes resulted in crashes, apparently these structures rot during RTL passes
// So...this pass runs just before expand to propagate the attribs that were
// dropped as best as possible by walking back through the stmts and
// tree
// FIXME: This may be because rvtt attributes were being mishandled?
static void
transform (function *fn)
{
  DUMP ("TT attrib pass on %s\n", function_name (fn));

  std::unordered_map<gimple *, tree> stmts;
  stmts.reserve (40);
  basic_block bb;
  FOR_EACH_BB_FN (bb, fn)
    for (gimple_stmt_iterator gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
      {
	tree lhs = gimple_get_lhs (gsi_stmt (gsi));
	if (lhs != NULL_TREE && TREE_CODE (lhs) == SSA_NAME && POINTER_TYPE_P (TREE_TYPE (lhs)))
	  {
	    tree type = TREE_TYPE (lhs);
	    if (!lookup_attribute ("rvtt_l1_ptr", TYPE_ATTRIBUTES (type))
		&& !lookup_attribute ("rvtt_reg_ptr", TYPE_ATTRIBUTES (type)))
	      {
		DUMP ("no attrib, searching\n");
		if (tree result = process_node (stmts, gsi_stmt (gsi)))
		  {
		    type = build_type_attribute_variant (type, TYPE_ATTRIBUTES (TREE_TYPE (result)));
		    TREE_TYPE (lhs) = type;
		    DUMP ("found one , adding attribute\n");
		  }
	      }
	    else
	      DUMP ("found attrib, skipping\n");
	  }
      }
}

namespace {

const pass_data pass_data_rvtt_attrib =
{
  GIMPLE_PASS, /* type */
  "rvtt_attrib", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_attrib : public gimple_opt_pass
{
public:
  pass_rvtt_attrib (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_attrib, ctxt)
  {}

  virtual unsigned int execute (function *);
}; // class pass_rvtt_attrib

} // anon namespace

/* Entry point to rvtt_attrib pass.	*/
unsigned int
pass_rvtt_attrib::execute (function *fun)
{
  if (TARGET_XTT_TENSIX_WH)
    transform (fun);

  return 0;
}

gimple_opt_pass *
make_pass_rvtt_attrib (gcc::context *ctxt)
{
  return new pass_rvtt_attrib (ctxt);
}
