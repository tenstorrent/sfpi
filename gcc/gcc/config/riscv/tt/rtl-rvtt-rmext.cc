#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "cfghooks.h"
#include "df.h"
#include "memmodel.h"
#include "tm_p.h"
#include "insn-config.h"
#include "regs.h"
#include "emit-rtl.h"
#include "recog.h"
#include "cgraph.h"
#include "tree-pretty-print.h" /* for dump_function_header */
#include "varasm.h"
#include "insn-attr.h"
#include "conditions.h"
#include "flags.h"
#include "output.h"
#include "except.h"
#include "rtl-error.h"
#include "toplev.h" /* exact_log2, floor_log2 */
#include "reload.h"
#include "intl.h"
#include "cfgrtl.h"
#include "debug.h"
#include "tree-pass.h"
#include "tree-ssa.h"
#include "cfgloop.h"
#include "stringpool.h"
#include "attribs.h"
#include "asan.h"
#include "rtl-iter.h"
#include "print-rtl.h"
#include "function-abi.h"
#include <vector>
#include <unordered_map>
#include "rvtt.h"

#define DUMP(...) //fprintf(stderr, __VA_ARGS__)

using namespace std;

// The insn_link/log links code below was shamelessly copied from combine.c

struct insn_link {
  rtx_insn *insn;
  unsigned int regno;
  struct insn_link *next;
};

static struct insn_link **uid_log_links;
static int max_uid_known;

static inline int
insn_uid_check (const_rtx insn)
{
  int uid = INSN_UID (insn);
  gcc_checking_assert (uid <= max_uid_known);
  return uid;
}

#define LOG_LINKS(INSN)		(uid_log_links[insn_uid_check (INSN)])
#define FOR_EACH_LOG_LINK(L, INSN)				\
  for ((L) = LOG_LINKS (INSN); (L); (L) = (L)->next)
/* Links for LOG_LINKS are allocated from this obstack.	 */

static struct obstack insn_link_obstack;

/* Allocate a link.  */

static inline struct insn_link *
alloc_insn_link (rtx_insn *insn, unsigned int regno, struct insn_link *next)
{
  struct insn_link *l
    = (struct insn_link *) obstack_alloc (&insn_link_obstack,
					  sizeof (struct insn_link));
  l->insn = insn;
  l->regno = regno;
  l->next = next;
  return l;
}

/* Return false if we do not want to (or cannot) combine DEF.  */
static bool
can_combine_def_p (df_ref def)
{
  /* Do not consider if it is pre/post modification in MEM.  */
  if (DF_REF_FLAGS (def) & DF_REF_PRE_POST_MODIFY)
    return false;

  unsigned int regno = DF_REF_REGNO (def);

  /* Do not combine frame pointer adjustments.	*/
  if ((regno == FRAME_POINTER_REGNUM
       && (!reload_completed || frame_pointer_needed))
      || (!HARD_FRAME_POINTER_IS_FRAME_POINTER
	  && regno == HARD_FRAME_POINTER_REGNUM
	  && (!reload_completed || frame_pointer_needed))
      || (FRAME_POINTER_REGNUM != ARG_POINTER_REGNUM
	  && regno == ARG_POINTER_REGNUM && fixed_regs[regno]))
    return false;

  return true;
}

/* Return false if we do not want to (or cannot) combine USE.  */
static bool
can_combine_use_p (df_ref use)
{
  /* Do not consider the usage of the stack pointer by function call.  */
  if (DF_REF_FLAGS (use) & DF_REF_CALL_STACK_USAGE)
    return false;

  return true;
}

/* Fill in log links field for all insns.  */

static void
create_log_links (void)
{
  basic_block bb;
  rtx_insn **next_use;
  rtx_insn *insn;
  df_ref def, use;

  next_use = XCNEWVEC (rtx_insn *, max_reg_num ());

  /* Pass through each block from the end, recording the uses of each
     register and establishing log links when def is encountered.
     Note that we do not clear next_use array in order to save time,
     so we have to test whether the use is in the same basic block as def.

     There are a few cases below when we do not consider the definition or
     usage -- these are taken from original flow.c did. Don't ask me why it is
     done this way; I don't know and if it works, I don't want to know.	 */

  FOR_EACH_BB_FN (bb, cfun)
    {
      FOR_BB_INSNS_REVERSE (bb, insn)
	{
	  if (!NONDEBUG_INSN_P (insn))
	    continue;

	  /* Log links are created only once.  */
	  gcc_assert (!LOG_LINKS (insn));

	  FOR_EACH_INSN_DEF (def, insn)
	    {
	      unsigned int regno = DF_REF_REGNO (def);
	      rtx_insn *use_insn;

	      if (!next_use[regno])
		continue;

	      if (!can_combine_def_p (def))
		continue;

	      use_insn = next_use[regno];
	      next_use[regno] = NULL;

	      if (BLOCK_FOR_INSN (use_insn) != bb)
		continue;

	      /* flow.c claimed:

		 We don't build a LOG_LINK for hard registers contained
		 in ASM_OPERANDs.  If these registers get replaced,
		 we might wind up changing the semantics of the insn,
		 even if reload can make what appear to be valid
		 assignments later.  */
	      if (regno < FIRST_PSEUDO_REGISTER
		  && asm_noperands (PATTERN (use_insn)) >= 0)
		continue;

	      /* Don't add duplicate links between instructions.  */
	      struct insn_link *links;
	      FOR_EACH_LOG_LINK (links, use_insn)
		if (insn == links->insn && regno == links->regno)
		  break;

	      if (!links)
		LOG_LINKS (use_insn)
		  = alloc_insn_link (insn, regno, LOG_LINKS (use_insn));
	    }

	  FOR_EACH_INSN_USE (use, insn)
	    if (can_combine_use_p (use))
	      next_use[DF_REF_REGNO (use)] = insn;
	}
    }

  free (next_use);
}

typedef unordered_map<rtx_insn *, vector<rtx_insn *>> insn_map;

// Build up a map of insns from a def to all of its uses
// This is modeled after create_log_links.  TODO: replace create_log_link with
// this routine for the other use cases
static void
create_links (function *fn, insn_map& map)
{
  basic_block bb;
  rtx_insn *insn;

  vector<vector<rtx_insn *>> all_uses;
  all_uses.resize(max_reg_num());

  // General algorithm doesn't care about BB order, printing is easier if the
  // BBs and insns are in the same order
  FOR_EACH_BB_REVERSE_FN (bb, fn)
    {
      FOR_BB_INSNS_REVERSE (bb, insn)
	{
	  if (!NONDEBUG_INSN_P (insn))
	    continue;

	  // This can be a load and have null def for example with
	  // asm_operands defining multiple defs
	  df_ref def = df_single_def(DF_INSN_INFO_GET(insn));
	  if (def != nullptr)
	    {
	      unsigned int regno = DF_REF_REGNO (def);
	      auto& uses = all_uses[regno];
	      map.insert(pair<rtx_insn *, vector<rtx_insn *>>(insn, uses));
	    }

	  FOR_EACH_INSN_DEF (def, insn)
	    {
	      unsigned int regno = DF_REF_REGNO (def);
	      all_uses[regno].resize(0);
	    }

	  df_ref use;
	  FOR_EACH_INSN_USE (use, insn)
	    {
	      unsigned int regno = DF_REF_REGNO (use);
	      all_uses[regno].push_back(insn);
	    }
	}

      for (auto &uses : all_uses)
	{
	  uses.resize(0);
	}
    }

  all_uses.resize(0);
}

/* Return TRUE if INSN is deleted.  */
static bool
check_extend(rtx_insn *insn)
{
  rtx pat = PATTERN (insn);

  if (GET_CODE (pat) != SET)
    return false;

  int which = GET_CODE (SET_SRC (pat));
  if (which != SIGN_EXTEND && which != ZERO_EXTEND)
    return false;

  DUMP ("  found a %s, ", insn_data[INSN_CODE(insn)].name);

  struct insn_link *links;
  int count = 0;
  rtx_insn *load_insn = nullptr;
  FOR_EACH_LOG_LINK (links, insn)
    {
      count++;
      rtx lpat = PATTERN (links->insn);
      if (GET_CODE (lpat) == SET
	  && REG_P (SET_DEST (lpat))
	  && GET_CODE (SET_SRC (lpat)) == MEM
	  // This condition is safe, is it over re-strictive?
	  && GET_MODE (SET_DEST (lpat)) == GET_MODE (XEXP (SET_SRC (pat), 0))
	  && dead_or_set_p (insn, SET_DEST (lpat)))
	load_insn = links->insn;
    }

  if (!load_insn || count != 1)
    {
      DUMP ("could not merge with load\n");
      return false;
    }

  rtx lpat = PATTERN (load_insn);
  rtx extend = which == SIGN_EXTEND ? gen_rtx_SIGN_EXTEND (SImode, SET_SRC (lpat))
    : gen_rtx_ZERO_EXTEND (SImode, SET_SRC (lpat));
  
  if (validate_change (load_insn, &SET_DEST (lpat), SET_DEST (pat), true)
      && validate_change (load_insn, &SET_SRC (lpat), extend, true)
      && apply_change_group ())
    {
      DUMP ("merged load, deleted extend\n");
      set_insn_deleted (insn);
      return true;
    }

  // Huh?
  gcc_unreachable ();
}

/* Return true if INSN is deleted.  */

static bool
check_store(insn_map& map, rtx_insn *insn)
{
  rtx pat = PATTERN (insn);
  if (GET_CODE (pat) != SET
      || GET_CODE (SET_DEST (pat)) != REG
      || (GET_CODE (SET_SRC (pat)) != ZERO_EXTEND
	  && GET_CODE (SET_SRC (pat)) != SIGN_EXTEND)
      || !SUBREG_P (XEXP (SET_SRC (pat), 0)))
    return false;

  machine_mode mode = GET_MODE (XEXP (SET_SRC (pat), 0));
  DUMP ("  found a %s %s, ", GET_MODE_NAME (mode), insn_data[INSN_CODE (insn)].name);

  const auto& uses = map.find (insn);
  if (uses == map.end ())
    {
      DUMP ("with no uses\n");
      return false;
    }

  rtx dst = SET_DEST (pat);
  unsigned int dstreg = REGNO (SET_DEST (pat));
  bool all_uses_are_stores = true;
  rtx_insn *last_store = nullptr;
  for (rtx_insn *use : uses->second)
    {
      rtx upat = PATTERN (use);

      if (GET_CODE (upat) == SET
	  && GET_CODE (SET_DEST (upat)) == MEM
	  && SUBREG_P (SET_SRC (upat))
	  && GET_MODE (SET_DEST (upat)) == mode
	  && REGNO (SUBREG_REG (SET_SRC (upat))) == dstreg)
	{
	  if (dead_or_set_p (use, dst))
	    last_store = use;
	}
      else
	all_uses_are_stores = false;
    }

  if (!all_uses_are_stores || !last_store)
    {
      DUMP ("has either a non-store use or no store\n");
      return false;
    }

  // Update all uses
  bool successful = true;
  for (rtx_insn *use : uses->second)
    {
      successful = validate_change (use, &SUBREG_REG (SET_SRC (PATTERN (use))),
				   SUBREG_REG (XEXP (SET_SRC (pat), 0)), true);
      if (!successful)
	break;
    }

  if (successful)
    {
      // Update regnotes of last use
      rtx *link;
      for (link = &REG_NOTES (last_store); *link; link = &XEXP (*link, 1))
	if (REG_NOTE_KIND (*link) == REG_DEAD
	    && REG_P (XEXP (*link, 0))
	    && REGNO (XEXP (*link, 0)) <= dstreg
	    && END_REGNO (XEXP (*link, 0)) > dstreg)
	  break;

      gcc_assert (*link);
      successful = validate_change (last_store, link,
				    find_reg_note (insn, REG_DEAD,
						   SUBREG_REG (XEXP (SET_SRC (pat), 0))),
				    true);
    }

  if (successful && apply_change_group ())
    {
      DUMP("merged %zu store(s), deleted extend\n", uses->second.size());
      set_insn_deleted (insn);
      return true;
    }

  // HUH?
  gcc_unreachable ();
}

// Look for a left shift after a zero/sign extend
// Replace with a shift left/shift right (saves a shift)
//
// Haven't seen a shift before by a zero/sign extend, could
// handle that case as well
static void
check_shift(rtx_insn *insn)
{
  rtx pat = PATTERN (insn);

  if (GET_CODE(pat) == SET &&
      GET_CODE(SET_DEST(pat)) == REG &&
      GET_CODE(SET_SRC(pat)) == ASHIFT &&
      GET_CODE(XEXP(SET_SRC(pat), 0)) == REG &&
      GET_CODE(XEXP(SET_SRC(pat), 1)) == CONST_INT &&
      dead_or_set_p(insn, XEXP(SET_SRC(pat), 0)))
    {
      DUMP("  found an ashift");

      struct insn_link *links;
      rtx_insn *extend_insn = nullptr;
      rtx src = XEXP(SET_SRC(pat), 0);
      FOR_EACH_LOG_LINK (links, insn)
	{
	  rtx lpat = PATTERN(links->insn);
	  if (GET_CODE(lpat) == SET &&
	      GET_CODE(SET_DEST(lpat)) == REG &&
	      GET_MODE(SET_DEST(lpat)) == SImode &&
	      REGNO(SET_DEST(lpat)) == REGNO(src) &&
	      (GET_CODE(SET_SRC(lpat)) == ZERO_EXTEND ||
	       GET_CODE(SET_SRC(lpat)) == SIGN_EXTEND) &&
	      SUBREG_P(XEXP(SET_SRC(lpat), 0)) &&
	      GET_MODE(XEXP(SET_SRC(lpat), 0)) == HImode)
	    {
	      extend_insn = links->insn;
	    }
	}
      if (extend_insn != nullptr)
	{
	  DUMP(", merging with subsequent extend\n");

	  // We know we are going from HI to SI mode...
	  int amt = INTVAL(XEXP(SET_SRC(pat), 1));
	  rtx ashft = gen_rtx_ASHIFT(SImode, XEXP(XEXP(SET_SRC(PATTERN(extend_insn)), 0), 0), GEN_INT(16));
	  rtx shftrt = GET_CODE(SET_SRC(PATTERN(extend_insn))) == ZERO_EXTEND ?
	    gen_rtx_LSHIFTRT(SImode, XEXP(SET_SRC(pat), 0), GEN_INT(16 - amt)) :
	    gen_rtx_ASHIFTRT(SImode, XEXP(SET_SRC(pat), 0), GEN_INT(16 - amt));

	  if (!(validate_change(extend_insn, &(SET_SRC(PATTERN(extend_insn))), ashft, true) &&
		validate_change(insn, &(SET_SRC(PATTERN(insn))), shftrt, true) &&
		apply_change_group()))
	    {
	      debug_rtx(insn);
	      debug_rtx(extend_insn);
	      gcc_unreachable();
	    }
	}
      else
	{
	  DUMP(", no viable matching extend\n");
	}
    }
}

// This pass removes unnecessary sign/zero extension operations
//
// This shows up in our code a lot because, at present, we use volatiles
// heavily and GCC bails out on merging zero/sign extend ops with loads/stores
// when the address is volatile.
//
// The 2 (most common) cases handled are:
// 1) A load followed immediately by a zero/sign ext
// 2) A zero/sign ext followed immediately by a store
//
// There are 2 more less common more complex cases that aren't handled at
// present:
// 1) A zero/sign ext followed by multiple stores of the result
// 2) A zero/sign ext followed by a store and another (safe) use of the result
static void
transform(function *fn)
{
  DUMP("TT Riscv opt pass on: %s\n", function_name(fn));

  basic_block bb;

  max_uid_known = get_max_uid ();
  uid_log_links = XCNEWVEC (struct insn_link *, max_uid_known + 1);
  gcc_obstack_init (&insn_link_obstack);
  df_set_flags (DF_LR_RUN_DCE);
  df_note_add_problem ();
  df_analyze();
  create_log_links ();
  insn_map map;
  create_links (fn, map);

  FOR_EACH_BB_FN (bb, fn)
    {
      rtx_insn *insn;
      FOR_BB_INSNS (bb, insn)
	{
	  if (NONDEBUG_INSN_P(insn))
	    {
	      if (!check_extend(insn)
		  && !check_store(map, insn))
		check_shift(insn);
	    }
	}
    }

  obstack_free (&insn_link_obstack, NULL);
  free (uid_log_links);
}

namespace {

const pass_data pass_data_rvtt_rmext =
{
  RTL_PASS, /* type */
  "rvtt_rmext", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  TODO_df_finish, /* todo_flags_finish */
};

class pass_rvtt_rmext : public rtl_opt_pass
{
public:
  pass_rvtt_rmext (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_rvtt_rmext, ctxt)
  {
  }

  /* opt_pass methods: */
  virtual bool gate (function *) override
  {
    return riscv_tt_opt_extend > 0;
  }

  virtual unsigned execute (function *cfn) override
    {
      transform (cfn);
      return 0;
    }
}; // class pass_rvtt_rmext

} // anon namespace

rtl_opt_pass *
make_pass_rvtt_rmext (gcc::context *ctxt)
{
  return new pass_rvtt_rmext (ctxt);
}
