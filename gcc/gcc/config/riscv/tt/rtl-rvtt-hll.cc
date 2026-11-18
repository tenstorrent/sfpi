/* Pass to work around GS' memory aribtration bug
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
#define INCLUDE_VECTOR
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "rtl.h"
#include "df.h"
#include "tree-pass.h"
#include "cfgbuild.h"
#include "insn-attr.h"
#include "rvtt.h"
#include <unordered_map>

#define DUMP(...) //fprintf(stderr, __VA_ARGS__)

const int stack_ptr_regno = STACK_POINTER_REGNUM;

static int top_of_bb_n_moved = 0;
static const int l1_load_shadow = 6;
static const int reg_load_shadow = 4;
static const int local_load_shadow = 1;

// A BB can resolve its parents' HLL loads by:
//  1) reading the incoming war reg
//  2) issuing a load (updates HLL or otherwise) and reading the result
//
// #2 is a stronger result as any parent BB with an ll_count low enough can be
// resolved. If the BB reads the incoming reg, then other parents must
// depend on the same reg or resolve the HLL load themselves
// The present implementation  doesn't try to get correct results for both #1
// and #2, instead it tracks the first workaround hit and classifies it as one
// or the other.  Future work if valuable...
struct bb_entry {
  bool inited;                      // true once populated w/ incoming data
  int ll_incoming;                  // #ll for first parent BB to get to this BB
  std::vector<int> hll_war_reg_incoming; // regs passed in that can resolve the parent
  int ll_before_resolve_all;        // #ll before all WARs are resolved (-1 if NA)
};

typedef std::vector<bb_entry> bb_data;

// Copied (and modified) from rtl.c.  This is potentially subject to rot.
//
// That routine didn't quite do what was needed - primarily because it would
// call RETURN's JUMP_INSN and the client of this code needs to ignore jumps
// between BBs but act on RETURNs.
static enum rtx_code
my_classify_insn (rtx x)
{
  if (LABEL_P (x))
    return CODE_LABEL;
  if (GET_CODE (x) == CALL)
    return CALL_INSN;
  if (ANY_RETURN_P (x))
    return RETURN;
  if (GET_CODE (x) == ASM_OPERANDS && ASM_OPERANDS_LABEL_VEC (x))
    return JUMP_INSN;
  if (GET_CODE (x) == SET)
    {
      if (GET_CODE (SET_DEST (x)) == PC)
	return JUMP_INSN;
      else if (GET_CODE (SET_SRC (x)) == CALL)
	return CALL_INSN;
      else
	return INSN;
    }

  if (GET_CODE (x) == PARALLEL)
    {
      int j;
      bool has_return_p = false;
      for (j = XVECLEN (x, 0) - 1; j >= 0; j--)
	{
	  if (GET_CODE (XVECEXP (x, 0, j)) == CALL)
	    return CALL_INSN;
	  else if (ANY_RETURN_P (XVECEXP (x, 0, j)))
	    has_return_p = true;
	  else if (GET_CODE (XVECEXP (x, 0, j)) == SET
		   && GET_CODE (SET_DEST (XVECEXP (x, 0, j))) == PC)
	    return JUMP_INSN;
	  else if (GET_CODE (XVECEXP (x, 0, j)) == SET
		   && GET_CODE (SET_SRC (XVECEXP (x, 0, j))) == CALL)
	    return CALL_INSN;
	}

      if (has_return_p)
	return RETURN;
      if (GET_CODE (XVECEXP (x, 0, 0)) == ASM_OPERANDS
	  && ASM_OPERANDS_LABEL_VEC (XVECEXP (x, 0, 0)))
	return JUMP_INSN;
    }

  return INSN;
}

static bool
load_mem_p(rtx pat)
{
  if (GET_CODE (pat) != SET)
    return false;

  if (GET_CODE (SET_DEST (pat)) != REG)
    return false;

  rtx src = SET_SRC (pat);
  if (GET_CODE (src) == CALL
      || GET_CODE (src) == ASM_OPERANDS)
    return false;

  return contains_mem_rtx_p (src);
}

static bool
stack_load_mem_p(rtx pat)
{
  return load_mem_p (pat)
    && refers_to_regno_p (stack_ptr_regno, pat);
}

static bool
store_mem_p(rtx pat)
{
  if (GET_CODE (pat) != SET)
    return false;

  if (GET_CODE (SET_SRC (pat)) != REG)
    return false;

  rtx dst = SET_DEST(pat);
  if (GET_CODE (dst) == CALL
      || GET_CODE (dst) == ASM_OPERANDS)
    return false;

  return contains_mem_rtx_p (dst);
}

static bool
nonstack_store_p(rtx pat)
{
  if (!store_mem_p (pat))
    return false;
  
  return !refers_to_regno_p (stack_ptr_regno, pat);
}

static bool
stack_store_p(rtx pat)
{
  return
    GET_CODE(pat) == SET &&
    contains_mem_rtx_p(SET_DEST(pat)) &&
    refers_to_regno_p(stack_ptr_regno, pat);
}

static bool
volatile_store_p(rtx pat)
{
  return store_mem_p(pat) && MEM_VOLATILE_P(SET_DEST(pat));
}

static bool
volatile_load_p(rtx pat)
{
  return load_mem_p(pat) && MEM_VOLATILE_P(SET_SRC(pat));
}

static bool
get_mem_reg_and_offset(rtx pat, int *reg, int *offset)
{
  if (GET_CODE(pat) == ZERO_EXTEND ||
      GET_CODE(pat) == SIGN_EXTEND)
    {
      pat = XEXP(pat, 0);
    }
  if (GET_CODE(pat) == ASM_OPERANDS)
    {
      return false;
    }
  gcc_assert(MEM_P(pat));

  if (REG_P(XEXP(pat, 0)))
    {
      *reg = REGNO(XEXP(pat, 0));;
      *offset = 0;
    }
  else if (GET_CODE(XEXP(pat, 0)) != PLUS &&
	   GET_CODE(XEXP(pat, 0)) != LO_SUM)
    {
      return false;
    }
  else
    {
      gcc_assert((GET_CODE(XEXP(pat, 0)) == PLUS ||
		  GET_CODE(XEXP(pat, 0)) == LO_SUM) &&
		 REG_P(XEXP(XEXP(pat, 0), 0)));

      *offset = CONST_INT_P((XEXP(XEXP(pat, 0), 1))) ? INTVAL(XEXP(XEXP(pat, 0), 1)) : 0;
      *reg = REGNO(XEXP(XEXP(pat, 0), 0));
    }

  return true;
}

class load_type {
 private:
  int lt;

 public:
  static const int ll_load = 1;
  static const int l1_load = 2;
  static const int reg_load = 4;
  static const int load_use = 8;

  load_type() : lt(0) {}
  void add_flavor(int flavor) { lt |= flavor; }
  void add_flavor(load_type& in) { lt |= in.lt; }
  bool ll_load_p() { return (lt & ll_load) != 0; }
  bool l1_load_p() { return (lt & l1_load) != 0; }
  bool reg_load_p() { return (lt & reg_load) != 0; }
  bool use_p() { return (lt & load_use) != 0; }
  bool hll_p() { return (lt & (l1_load | reg_load)) != 0; }
  bool any_load_p() { return (lt & (ll_load | l1_load | reg_load)) != 0; }

  int get_shadow()
  {
    return (l1_load_p()) ? l1_load_shadow : ((reg_load_p()) ? reg_load_shadow : local_load_shadow);
  }

  const char *get_name()
  {
    if (ll_load_p()) return "ll";
    if (l1_load_p()) return "l1";
    if (reg_load_p()) return "rl";
    return "??";
  }
};

struct load_def;
struct sched_insn_data;

struct load_base {
  load_type init_type;        // (ll/l1/reg | use)
  int init_insn_count;        // up from the bottom of the bb
  sched_insn_data *id;        // pointer back to the, eg, adjustable insn_count
  rtx_insn *insn;             // the load insn
};

struct load_dep {
  struct load_base base;
  bool moved;
  load_def *def;
};

struct load_def {
  struct load_base base;
  basic_block bb;             // bb for insn
  std::vector<load_dep> uses;      // each of the uses of this reg
};

typedef std::vector<struct load_dep> load_deps;

struct sched_insn_data {
  load_type type;
  int insn_count;
  load_def *def;             // only used for use
};

static std::unordered_map<rtx_insn *, sched_insn_data> insn_count_map;

// Build up a vector of load_defs such that:
//  - there is one entry per load in reverse order
//  - each entry points to the insn, all the uses of the def of that insn
//    and the insn count (up from the bottom of the BB)
// Also create the a list of registers loaded but not used (open uses) at
// the end of the bb
static void
create_load_data (function *fn,
		  std::vector<load_def>& load_defs,
		  std::vector<std::vector<load_def *>>& bb_reg_open_defs)
{
  basic_block bb;
  rtx_insn *insn;
  df_ref use;

  std::vector<load_deps> all_uses;
  all_uses.resize(FIRST_PSEUDO_REGISTER);
  insn_count_map.clear();

  // General algorithm doesn't care about BB order, printing is easier if the
  // BBs and insns are in the same order
  FOR_EACH_BB_REVERSE_FN (bb, fn)
    {
      int insn_count = 0;
      FOR_BB_INSNS_REVERSE (bb, insn)
	{
	  if (!NONDEBUG_INSN_P (insn))
	    continue;

	  rtx insn_pat = PATTERN(insn);
	  // This can be a load and have null def for example with
	  // asm_operands defining multiple defs
	  df_ref def = df_single_def(DF_INSN_INFO_GET(insn));
	  if (def != nullptr)
	    {
	      if (load_mem_p(insn_pat))
		{
		  unsigned int regno = DF_REF_REGNO (def);
		  auto& uses = all_uses[regno];

		  struct load_def ld;
		  ld.base.insn = insn;
		  ld.bb = bb;
		  if (rvtt_l1_load_p(insn_pat)) ld.base.init_type.add_flavor(load_type::l1_load);
		  else if (rvtt_reg_load_p(insn_pat)) ld.base.init_type.add_flavor(load_type::reg_load);
		  else ld.base.init_type.add_flavor(load_type::ll_load);

		  ld.uses = uses;
		  ld.base.init_insn_count = (GET_CODE(insn_pat) == USE) ? insn_count - 1 : insn_count;
		  load_defs.push_back(ld);
		}
	    }

	  FOR_EACH_INSN_DEF (def, insn)
	    {
	      unsigned int regno = DF_REF_REGNO (def);
	      all_uses[regno].resize(0);
	    }

	  FOR_EACH_INSN_USE (use, insn)
	    {
	      unsigned int regno = DF_REF_REGNO (use);

	      struct load_dep hl_use;
	      hl_use.base.insn = insn;
	      hl_use.moved = false;
	      hl_use.base.init_insn_count = (GET_CODE(insn_pat) == USE) ? insn_count - 1 : insn_count;
	      hl_use.base.init_type.add_flavor(load_type::load_use);
	      all_uses[regno].push_back(hl_use);
	    }

	  if (GET_CODE(insn_pat) != USE)
	    {
	      insn_count++;
	    }
	}

      for (auto &uses : all_uses)
	{
	  uses.resize(0);
	}
    }

  // Note: a use could also be a def
  // store insn_count once and point back
  // XXXX a use of 2 loads will only point back to one of the loads (semi-rare)
  for (auto& ld : load_defs)
    {
      sched_insn_data lsid;
      auto result = insn_count_map.insert(std::pair<rtx_insn *, sched_insn_data>(ld.base.insn, lsid));
      ld.base.id = &result.first->second;
      result.first->second.insn_count = ld.base.init_insn_count;
      result.first->second.type.add_flavor(ld.base.init_type);

      if (ld.base.init_type.hll_p() && ld.uses.size() == 0)
	{
	  edge_iterator ei;
	  edge e;
	  FOR_EACH_EDGE(e, ei, ld.bb->succs)
	    {
	      bb_reg_open_defs[e->dest->index].push_back(&ld);
	    }
	}

      for (auto& use : ld.uses)
	{
	  use.def = &ld;
	  sched_insn_data usid;
	  auto result = insn_count_map.insert(std::pair<rtx_insn *, sched_insn_data>(use.base.insn, usid));
	  use.base.id = &result.first->second;
	  result.first->second.insn_count = use.base.init_insn_count;
	  result.first->second.type.add_flavor(use.base.init_type);
	  result.first->second.def = use.def;
	}
    }

  all_uses.resize(0);
}

// Find the instruction count (ic) when load in ld is resolved (-1 if not resolved)
static int
get_ic_of_resolution(std::vector<load_def>& load_defs, int which, load_def& ld,
		     bool go_to_the_end = false, load_dep **resolving_use = nullptr)
{
  // insn counts are numbered high to low
  int max = -1;
  for (auto &use : ld.uses)
    {
      if (use.base.id->insn_count > max)
	{
	  max = use.base.id->insn_count;
	  if (resolving_use != nullptr) *resolving_use = &use;
	}
    }

  int shadow = ld.base.id->type.get_shadow();
  shadow = go_to_the_end ? 1000000 : shadow;

  for (int j = which - 1; j >= 0; j--)
    {
      if (load_defs[j].bb != ld.bb) break;

      // See if a load (hll or not) occurs in the shadow of the current load
      if (load_defs[j].base.id->insn_count > ld.base.id->insn_count - shadow)
	{
	  // Bring in the max if this load is gating
	  for (auto &use : load_defs[j].uses)
	    {
	      if (use.base.id->insn_count > max)
		{
		  max = use.base.id->insn_count;
		  if (resolving_use != nullptr) *resolving_use = &use;
		}
	    }
	}
      else
	{
	  // We've moved out of the shadow range, stop looking for more
	  break;
	}
    }

  return max;
}

// XXXX clean this up, overlapping restrictions crept in
// is called with pat/base and base/pat
static bool
can_move_past_load_or_store(rtx base, rtx pat)
{
  // XXXX seems we could move a volatile load passed a stack load
  if ((volatile_store_p(base) || volatile_load_p(base)) &&
      (store_mem_p(pat) || load_mem_p(pat)))
    {
      return false;
    }

  if (load_mem_p(base))
    {
      if (store_mem_p(pat))
	{
	  int store_reg, store_offset;
	  int load_reg, load_offset;
	  if (!get_mem_reg_and_offset(SET_DEST(pat), &store_reg, &store_offset) ||
	      !get_mem_reg_and_offset(SET_SRC(base), &load_reg, &load_offset))
	    {
	      return false;
	    }

	  // Conservative, could check the size of the operation
	  // Also, relies on aligned accesses
	  if (store_reg == load_reg &&
	      store_offset < load_offset + 4 && store_offset >= load_offset)
	    {
	      return false;
	    }

	  return rvtt_store_has_restrict_p(pat) && !volatile_store_p(pat);
	}

      return !volatile_load_p(pat);
    }

  if (nonstack_store_p(base) && nonstack_store_p(pat))
    {
      return rvtt_store_has_restrict_p(base) || rvtt_store_has_restrict_p(pat);
    }

  return true;
}

static bool
movable(rtx_insn *insn)
{
  rtx pat = PATTERN(insn);
  int classify = my_classify_insn(pat);
  if (classify != INSN ||
      GET_CODE(pat) == USE ||
      GET_CODE(pat) == UNSPEC_VOLATILE ||
      GET_CODE(pat) == ASM_INPUT ||
      GET_CODE(pat) == ASM_OPERANDS)
    {
      return false;
    }

  return true;
}

static bool
can_move_past(rtx_insn *base, rtx_insn *query)
{
  df_ref base_def, query_def;

  if (!movable(base) || !movable(query)) return false;

  rtx base_pat = PATTERN(base);
  rtx query_pat = PATTERN(query);

  if (GET_CODE(base_pat) == PARALLEL ||
      GET_CODE(query_pat) == PARALLEL)
    {
      return false;
    }
  else if (!can_move_past_load_or_store(base_pat, query_pat) ||
	   !can_move_past_load_or_store(query_pat, base_pat))
    {
      return false;
    }

  FOR_EACH_INSN_DEF (query_def, query)
    {
      // If query defines what base defines
      FOR_EACH_INSN_DEF (base_def, base)
	{
	  if (DF_REF_REGNO(base_def) == DF_REF_REGNO(query_def))
	    {
	      return false;
	    }
	}

      // If query defines what base uses
      df_ref use;
      FOR_EACH_INSN_USE (use, base)
	{
	  if (DF_REF_REGNO(use) == DF_REF_REGNO(query_def))
	    {
	      return false;
	    }
	}
    }

  // If base defines what query uses
  FOR_EACH_INSN_DEF (base_def, base)
    {
      FOR_EACH_INSN_USE (query_def, query)
	{
	  if (DF_REF_REGNO(base_def) == DF_REF_REGNO(query_def))
	    {
	      return false;
	    }
	}
    }

  return true;
}

// The next two routines come from emit-rtl modified so that they don't run
// into the prologue/epilogue
static rtx_insn *
my_prev_nonnote_nondebug_insn_bb (rtx_insn *insn)
{
  rtx_insn *last = PREV_INSN(BB_HEAD(BLOCK_FOR_INSN(insn)));
  while (insn)
    {
      insn = PREV_INSN (insn);
      if (insn == 0 || insn == last)
	break;
      if (DEBUG_INSN_P (insn))
	continue;
      if (!NOTE_P (insn))
	break;
      if (NOTE_INSN_BASIC_BLOCK_P (insn))
	return NULL;
    }

  return insn;
}

static rtx_insn *
my_next_nonnote_nondebug_insn_bb (rtx_insn *insn)
{
  rtx_insn *last = NEXT_INSN(BB_END(BLOCK_FOR_INSN(insn)));
  while (insn)
    {
      insn = NEXT_INSN (insn);
      if (insn == 0 || insn == last)
	break;
      if (DEBUG_INSN_P (insn))
	continue;
      if (!NOTE_P (insn))
	break;
      if (NOTE_INSN_BASIC_BLOCK_P (insn))
	return NULL;
    }

  return insn;
}

static void
move_insn(rtx_insn *insn, rtx_insn *where, bool after)
{
  remove_insn(insn);
  SET_PREV_INSN(insn) = NULL_RTX;
  SET_NEXT_INSN(insn) = NULL_RTX;

  if (after)
    {
      add_insn_after(insn, where, nullptr);
    }
  else
    {
      add_insn_before(insn, where, nullptr);
    }
}

static void
adjust_insn_count(rtx_insn *insn, int amt)
{
  auto ic = insn_count_map.find(insn);
  if (ic != insn_count_map.end())
    {
      ic->second.insn_count += amt;
    }
}

static int
move_down(rtx_insn *use_insn, int how_far)
{
  if (use_insn == nullptr) return 0;

  rtx_insn *insn = NEXT_INSN(use_insn);
  rtx_insn *anchor = nullptr;

  basic_block bb = BLOCK_FOR_INSN(use_insn);
  int move_count = 0;
  while (insn && insn != NEXT_INSN(BB_END(bb)) && move_count < how_far)
    {
      if (NONDEBUG_INSN_P(insn))
	{
	  if (!can_move_past(use_insn, insn))
	    {
	      break;
	    }
	  anchor = insn;
	  move_count++;
	}
      insn = NEXT_INSN(insn);
    }

  DUMP("  moving use down %d (wanted %d)\n", move_count, how_far);
  if (anchor)
    {
      adjust_insn_count(use_insn, -move_count);
      adjust_insn_count(anchor, move_count);
      move_insn(use_insn, anchor, true);
    }

  return move_count;
}

static bool
can_push_up_one(rtx_insn *move_insn, rtx_insn *target_insn)
{
  auto move_insn_d = insn_count_map.find(move_insn);

  // If move_insn is a use, don't push it up passed its def's shadow
  if (move_insn_d != insn_count_map.end() &&
      move_insn_d->second.type.use_p())
    {
      int shadow = move_insn_d->second.def->base.id->type.get_shadow();
      if (move_insn_d->second.insn_count > move_insn_d->second.def->base.id->insn_count - shadow)
	{
	  // XXXXX unless moving this up allows something further down to move up as well
	  return false;
	}
    }

  return can_move_past(move_insn, target_insn);
}

static bool
can_push_down_one(rtx_insn *move_insn, rtx_insn *target_insn)
{
#if 0
  // Hurts slightly.  Hmm.
  auto move_insn_d = insn_count_map.find(move_insn);
  auto target_insn_d = insn_count_map.find(target_insn);

  // Don't move across another load, messes up schedule_shadows
  if (move_insn_d != insn_count_map.end() &&
      move_insn_d->second.type.hll_p() &&
      target_insn_d != insn_count_map.end() &&
      target_insn_d->second.type.any_load_p())
    {
      return false;
    }
#endif
  return can_move_past(move_insn, target_insn);
}

static bool
can_push_insnset_up_passed(rtx_insn *insn, rtx_insn *target_insn, bool can_cross_load)
{
  if (!can_cross_load)
    {
      auto target_insn_d = insn_count_map.find(target_insn);
      if (target_insn_d != insn_count_map.end() &&
	  target_insn_d->second.type.any_load_p())
	{
	  return false;
	}
    }

  while (insn != target_insn)
    {
      if (!can_push_up_one(insn, target_insn)) return false;

      insn = my_prev_nonnote_nondebug_insn_bb(insn);
    }

  return true;
}

static bool
can_push_insnset_down_passed(rtx_insn *insn, rtx_insn *target_insn)
{
  while (insn != target_insn)
    {
      if (!can_push_down_one(insn, target_insn)) return false;

      insn = my_next_nonnote_nondebug_insn_bb(insn);
    }

  return true;
}

static void
push_insnset_up_passed(rtx_insn *insn, rtx_insn *target_insn, int move_count)
{
  rtx_insn *where = target_insn;
  while (insn != target_insn)
    {
      rtx_insn *prev_insn = my_prev_nonnote_nondebug_insn_bb(insn);

      move_insn(insn, where, false);
      adjust_insn_count(insn, 1);
      where = insn;
      insn = prev_insn;
    }

  adjust_insn_count(target_insn, -move_count);
}

static void
push_insnset_down_passed(rtx_insn *insn, rtx_insn *target_insn, int move_count)
{
  rtx_insn *where = target_insn;
  while (insn != target_insn)
    {
      rtx_insn *next_insn = my_next_nonnote_nondebug_insn_bb(insn);

      move_insn(insn, where, true);
      adjust_insn_count(insn, -1);
      where = insn;
      insn = next_insn;
    }

  adjust_insn_count(target_insn, move_count);
}

// Try to move insn up how_far
// If it doesn't move on its own (conflict above), try moving sets of insns up
// to max_set_size up together (address calculations often occur just before a
// load)
static bool
push_load_up(rtx_insn *insn, int in_how_far, int max_set_size, bool can_cross_load)
{
  int set_size = 1;
  int how_far = in_how_far;
  rtx_insn *last_insn = PREV_INSN(BB_HEAD(BLOCK_FOR_INSN(insn)));
  while (set_size <= max_set_size &&
	 how_far > (set_size - 1))
    {
      rtx_insn *target_insn = insn;
      for (int i = 0; i < set_size && target_insn != last_insn && target_insn != nullptr; i++)
	{
	  target_insn = my_prev_nonnote_nondebug_insn_bb(target_insn);
	}
      if (target_insn == last_insn || target_insn == nullptr) break;

      // Try to move up to max_set_size insns
      // Don't push the last above the previous load (how_far > set_size - 1)
      if (can_push_insnset_up_passed(insn, target_insn, can_cross_load))
	{
	  push_insnset_up_passed(insn, target_insn, set_size);
	  how_far--;
	}
      else
	{
	  // Try to move a larger block
	  set_size++;
	}
    }

  return how_far != in_how_far;
}

static void
push_use_down(rtx_insn *insn, int in_how_far, int max_set_size)
{
  int set_size = 1;
  int how_far = in_how_far;
  rtx_insn *last_insn = NEXT_INSN(BB_END(BLOCK_FOR_INSN(insn)));
  while (set_size <= max_set_size &&
	 how_far > (set_size - 1))
    {
      rtx_insn *target_insn = insn;
      for (int i = 0; i < set_size && target_insn != last_insn && target_insn != nullptr; i++)
	{
	  target_insn = my_next_nonnote_nondebug_insn_bb(target_insn);
	}
      if (target_insn == last_insn || target_insn == nullptr) break;

      // Try to move up to max_set_size insns
      // Don't push the last above the previous load (how_far > set_size - 1)
      if (can_push_insnset_down_passed(insn, target_insn))
	{
	  push_insnset_down_passed(insn, target_insn, set_size);
	  how_far--;
	}
      else
	{
	  // Try to move a larger block
	  set_size++;
	}
    }
}

// Finds the gating use (earliest use within the shadow) and pushes it down.
// Doing so may expose a different use as a new gating use, so this iterates.
// We must be careful as 2 insns could repeatedly swap locations or a use
// could be pushed down out of the shadow but then the next use, when pushed
// down, could result in the previous use getting moved back into the shadow.
// Current implementaion just moves an insn once, and (hackily) tries to push
// a bit further if there are multiple uses (rare case).
//
// push_to is the insn_count to push the use down.  how_far isn't used here
// because the use insn can change depending on how this loop executes (ie, we
// need an absolute not relative position)
static void
push_gating_use_down(std::vector<load_def>& defs, int which, load_def& ld, int push_to)
{
  // Find gating insn
  load_dep *target_use = nullptr;
  int max = get_ic_of_resolution(defs, which, ld, false, &target_use);

  if (target_use != nullptr)
    {
      while (max > push_to && !target_use->moved)
	{
	  push_use_down(target_use->base.insn, target_use->base.id->insn_count - push_to, 3);
	  target_use->moved = true;

	  max = get_ic_of_resolution(defs, which, ld, false, &target_use);
	}
    }
}

// "Schedule" high latency loads
// This project nearly killed me - way too much effort for too little return.
// I kept at it as it always seemed there was an issue causing unexpected
// behavior that may improve the results when fixed.  I think that is still
// the case.  Ugh.
//
// This last iteration tries to "do no harm" by not moving things that benefit
// one load a the expense of another.  This means that load/use order are
// preserved.  The code first pushes hlls "up" as high as possible in the BB,
// then pushes their dependencies (uses) "down" as low as possible in the
// BB. (disabled)
//
// I've tried a few (simple) variations on this theme all of which produce
// results within noise of each other.  Seems either pushing uses down or
// loads up gains almost everything there is to gain.  I suspect that is a
// sign the compiler is already doing a decent job: improving one load just
// hurts another.
//
// Perhaps a "real" algorithm could do better, but I suspect there isn't much
// more to be had and enough time has been sunk on this already...
static void
schedule_hll(function *fn)
{
  DUMP("TT riscv GS rtl hll schedule pass on: %s\n", function_name(fn));

  std::vector<load_def> load_defs;

  // Registers defined in hlls not used within the BB
  std::vector<std::vector<load_def *>> bb_reg_open_defs;
  bb_reg_open_defs.resize(n_basic_blocks_for_fn(fn));

  create_load_data(fn, load_defs, bb_reg_open_defs);
  load_def *prev_hll = nullptr;
  for (int i = load_defs.size() - 1; i >= 0; i--)
    {
      load_def& ld = load_defs[i];
      if (!ld.base.id->type.hll_p())
	{
	  continue;
	}
      DUMP("  processing hll load\n");

      int dist_to_gate = 100;
      int shadow = ld.base.id->type.get_shadow();
      if (prev_hll != nullptr)
	{
	  dist_to_gate = prev_hll->base.id->insn_count - ld.base.id->insn_count;
	  int ic_of_resolution = get_ic_of_resolution(load_defs, i, ld);
	  if (ic_of_resolution < ld.base.id->insn_count - shadow)
	    {
	      // If this ld doesn't benefit from being moved up, then
	      // don't move up if it is robbing the prior ld
	      // Do move up otherwise as this may help later loads
	      for (auto& use : prev_hll->uses)
		{
		  if (use.base.insn == ld.base.insn)
		    {
		      // Previous ld has an anti-dependence on ld
		      dist_to_gate -= prev_hll->base.id->type.get_shadow();
		    }
		}
	    }
	}

      push_load_up(ld.base.insn, dist_to_gate - 1, 3, false);
      push_gating_use_down(load_defs, i, ld, std::min(ld.base.id->insn_count - shadow, 0));

      prev_hll = &ld;
    }

  // Push uses at the start of a BB carried in from prior BB down
  // Only look at the first few insns of the BB
  basic_block bb;
  FOR_EACH_BB_FN (bb, fn)
    {
      if (bb_reg_open_defs[bb->index].size() > 0)
	{
	  int count = 0;
	  rtx_insn *insn = BB_HEAD(bb);
	  while (insn && insn != NEXT_INSN(BB_END(bb)) && count < l1_load_shadow)
	    {
	      if (NONDEBUG_INSN_P(insn))
		{
		  for (unsigned int i = 0; i < bb_reg_open_defs[bb->index].size(); i++)
		    {
		      load_def& ld = *bb_reg_open_defs[bb->index][i];
		      unsigned int regno = DF_REF_REGNO(df_single_def(DF_INSN_INFO_GET(ld.base.insn)));
		      df_ref use;
		      FOR_EACH_INSN_USE (use, insn)
			{
			  if (DF_REF_REGNO(use) == regno)
			    {
			      int shadow = ld.base.id->type.get_shadow();
			      int how_far = shadow - ld.base.id->insn_count - count;

			      // XXXXXX use push_gating_use_down
			      bool moved = (move_down(insn, how_far) != 0);
			      top_of_bb_n_moved += moved;
			      break;
			    }
			}
		    }
		  count++;
		}

	      insn = NEXT_INSN(insn);
	    }
	}
    }
}

static load_dep *
get_closest_use(std::vector<load_dep>& uses)
{
  int max = -1;
  load_dep *closest_use = nullptr;
  for (auto &use : uses)
    {
      if (use.base.id->insn_count > max)
	{
	  closest_use = &use;
	  max = use.base.id->insn_count;
	}
    }

  return closest_use;
}

// When the uses' of multiple loads falls out of order, scheduling runs into
// issues as sliding loads/uses around may not have the intended effect.  This
// pass schedules loads/uses that fall in the shadows of other hll loads to
// preserve the order of resolution.
static void
schedule_shadows(function *fn)
{
  DUMP("TT riscv GS rtl shadow cleanup schedule pass on: %s\n", function_name(fn));

  std::vector<load_def> load_defs;

  // Registers defined in hlls not used within the BB
  std::vector<std::vector<load_def *>> bb_reg_open_defs;
  bb_reg_open_defs.resize(n_basic_blocks_for_fn(fn));

  create_load_data(fn, load_defs, bb_reg_open_defs);

  for (int i = load_defs.size() - 1; i >= 0; i--)
    {
      load_def& ld = load_defs[i];

      if (!ld.base.id->type.hll_p()) continue;

      // Get the ic this ld is resolved
      load_dep *closest_use = get_closest_use(ld.uses);
      if (closest_use == nullptr) continue;
      int bb = ld.bb->index;
      int shadow = ld.base.id->type.get_shadow();
      int dist = ld.base.id->insn_count - closest_use->base.id->insn_count;
      if (dist < shadow)
	{
	  // Find lds in the shadow of this ld
	  int max = closest_use->base.id->insn_count;
	  load_dep *overall_closest_shadow_use = nullptr;
	  load_def *overall_closest_shadow_ld = nullptr;
	  for (int j = i - 1; j >= 0; j--)
	    {
	      if (bb != load_defs[j].bb->index ||
		  load_defs[j].base.id->insn_count < ld.base.id->insn_count - shadow) break;

	      // Get the ic this ld is resolved
	      load_dep *closest_shadow_use = get_closest_use(load_defs[j].uses);

	      if (closest_shadow_use != nullptr && closest_shadow_use->base.id->insn_count > max)
		{
		  max = closest_shadow_use->base.id->insn_count;
		  overall_closest_shadow_use = closest_shadow_use;
		  overall_closest_shadow_ld = &load_defs[j];
		}
	    }

	  if (overall_closest_shadow_use != nullptr &&
	      overall_closest_shadow_use->base.id->insn_count > closest_use->base.id->insn_count)
	    {
	      DUMP("  shuffling uses\n");

	      int dist_between_lds = ld.base.id->insn_count - overall_closest_shadow_ld->base.id->insn_count;
	      int dist_between_uses = overall_closest_shadow_use->base.id->insn_count - closest_use->base.id->insn_count;
	      if (!push_load_up(overall_closest_shadow_ld->base.insn, dist_between_lds, dist_between_lds, true))
		{
		  push_use_down(overall_closest_shadow_use->base.insn, dist_between_uses, dist_between_uses);
		}
	    }
	}
    }
}

static void
print_uses(const std::vector<load_def>& load_defs,
	   const std::vector<load_dep *>& deps,
	   int index, int init_ic, bool print_ic = false)
{
  int end_ic = (index == 0) ? 0 : load_defs[index - 1].base.id->insn_count;

  bool needcr = true;
  for (int ic = init_ic; ic > end_ic; ic--)
    {
      if (deps[ic] != nullptr)
	{
	  if (ic == init_ic && !print_ic)
	    {
	      fprintf(stderr, "\tuse ");
	    }
	  else
	    {
	      if (needcr && !print_ic)
		{
		  fprintf(stderr, "\n");
		  needcr = false;
		}
	      fprintf(stderr, "%4d:%4d\t\tuse ", ic, INSN_UID(deps[ic]->base.insn));
	    }

	  df_ref ruse;
	  FOR_EACH_INSN_USE (ruse, deps[ic]->base.insn)
	    {
	      int regno = DF_REF_REGNO(ruse);
	      fprintf(stderr, "%d ", regno);
	    }
	  needcr = false;
	  fprintf(stderr, "\n");
	}
    }

  if (needcr && !print_ic)
    {
      fprintf(stderr, "\n");
    }
}

static int
get_max_ic(basic_block bb, int base)
{
  int init_ic = 0;
  rtx_insn *insn = BB_HEAD(bb);
  while (insn && insn != NEXT_INSN(BB_END(bb)))
    {
      if (NONDEBUG_INSN_P(insn))
	{
	  auto matched = insn_count_map.find(insn);
	  if (matched != insn_count_map.end())
	    {
	      break;
	    }
	  if (GET_CODE(PATTERN(insn)) != USE)
	    {
	      init_ic++;
	    }
	}
      insn = NEXT_INSN(insn);
    }
  gcc_assert(insn != NEXT_INSN(BB_END(bb)));

  return init_ic + base;
}

static void
print_hll_schedule(function *fn)
{
  std::vector<load_def> load_defs;

  std::vector<std::vector<load_def *>> bb_reg_open_defs;
  bb_reg_open_defs.resize(n_basic_blocks_for_fn(fn));
  create_load_data(fn, load_defs, bb_reg_open_defs);

  int index = load_defs.size() - 1;
  fprintf(stderr, "HLL schedule dump for: %s\n", function_name(fn));
  if (index < 0) return;

  std::vector<load_dep *>deps;
  deps.reserve(1000);

  // Early out if this fn has 0 hlls
  bool any_hlls = false;
  for (auto& ld : load_defs)
    {
      if (ld.base.id->type.hll_p())
	{
	  any_hlls = true;
	  break;
	}
    }
  if (!any_hlls) return;

  basic_block bb;
  FOR_EACH_BB_FN (bb, fn)
    {
      load_def *ld = &load_defs[index];
      // Skip over initial local loads
      while (bb->index == ld->bb->index && !ld->base.id->type.hll_p())
	{
	  if (--index < 0) break;
	  ld = &load_defs[index];
	}

      // Skip this BB or exit if no hlls
      if (index < 0 || bb->index != ld->bb->index)
	{
	  fprintf(stderr, "BB:%d <none>\n", bb->index);
	  if (index < 0) return;
	  continue;
	}

      // Figure out max ic count in this BB
      int max_ic = get_max_ic(bb, ld->base.id->insn_count);
      deps.resize(max_ic + 1);
      for (int i = 0; i < max_ic; i++) deps[i] = nullptr;

      fprintf(stderr, "BB:%d (%d insns)\n", bb->index, max_ic);
      int ic = -1;
      do
	{
	  ic = ld->base.id->insn_count;
	  load_dep *res_use = nullptr;
	  int ic_of_resolution = get_ic_of_resolution(load_defs, index, *ld, true, &res_use);
	  if (ic_of_resolution >= 0)
	    {
	      deps[ic_of_resolution] = res_use;
	    }
	  if (ld->uses.size() > 0)
	    {
	      load_dep& use = ld->uses[ld->uses.size() - 1];
	      deps[use.base.id->insn_count] = &use;
	    }

	  unsigned int regno = DF_REF_REGNO(df_single_def(DF_INSN_INFO_GET(ld->base.insn)));
	  fprintf(stderr, "%4d:%4d\t%s%s %d",
		  ld->base.id->insn_count,
		  INSN_UID(ld->base.insn),
		  volatile_load_p(PATTERN(ld->base.insn)) ? "v" : "",
		  ld->base.id->type.get_name(),
		  regno);
	  print_uses(load_defs, deps, index, ic);

	  if (--index < 0) break;
	  ld = &load_defs[index];
	} while (bb->index == ld->bb->index);

      if (index < 0) break;

      print_uses(load_defs, deps, index, ic, true);
    }
}

namespace {

const pass_data pass_data_rvtt_hll =
{
  RTL_PASS, /* type */
  "rvtt_hll", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  TODO_df_finish, /* todo_flags_finish */
};

class pass_rvtt_hll : public rtl_opt_pass
{
private:

public:
  pass_rvtt_hll (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_rvtt_hll, ctxt)
  {
  }

  virtual bool gate (function *) override
  {
    return riscv_tt_opt_hll > 0;
  }

  /* opt_pass methods: */
  virtual unsigned execute (function *cfn) override
    {
      // Interesting, this cleans up some extraneous moves
      // Leave it in (found after disabling hll pass)
      df_set_flags (DF_LR_RUN_DCE);
      df_note_add_problem ();
      df_analyze();

      schedule_shadows(cfn);
      schedule_hll(cfn);

      return 0;
    }
}; // class pass_rvtt_hll

} // anon namespace

rtl_opt_pass *
make_pass_rvtt_hll (gcc::context *ctxt)
{
  return new pass_rvtt_hll (ctxt);
}
