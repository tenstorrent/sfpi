// sfpu_bool.h
//
// Implements the recursive "store up and repay" boolean tree for the sfpu simulator
#pragma once

#include <vector>

using namespace sfpu;
using namespace sfpi;
using namespace std;

class SFPUConditional {
public:
    enum class opType {
        opFcmpS,
        opFcmpV,
        opIcmpS,
        opIcmpV,
        opOr,
        opAnd,
    } operation;

    const __rvtt_vec_t v1;
    const __rvtt_vec_t v2;
    const unsigned  int imm;
    const int mod;
    const int op_a;
    const int op_b;
    bool negated;

    SFPUConditional() : v1(), v2(), imm(0), mod(0), op_a(0), op_b(0), negated(false) {}
    SFPUConditional(opType t, const __rvtt_vec_t& v, unsigned int f, int m) : operation(t), v1(v), v2(), imm(f), mod(m), op_a(0), op_b(0), negated(false) {}
    SFPUConditional(opType t, const __rvtt_vec_t& in1, const __rvtt_vec_t& in2, int m) : operation(t), v1(in1), v2(in2), imm(0), mod(m), op_a(0), op_b(0), negated(false) { }
    SFPUConditional(opType t, int a, int b) : operation(t), v1(), v2(), imm(0), mod(0), op_a(a), op_b(b), negated(false) {}

    inline void negate() { negated = !negated; }
    inline bool issues_compc(bool negate) const;
    inline opType get_boolean_type(bool negate) const;
    inline bool is_boolean() const { return (operation == opType::opOr) || (operation == opType::opAnd); }
    inline int negate_mod() const;

    static void emit_conditional(int w, bool negate);
    static void emit_boolean_tree(int w, bool negate);
};

static vector<SFPUConditional> sfpu_conditionals;

inline int SFPUConditional::negate_mod() const
{
    int op = mod & SFPXCMP_MOD1_CC_MASK;
    int new_op = SFPXCMP_MOD1_CC_GTE;

    switch (op) {
    case SFPXCMP_MOD1_CC_LT:
        new_op = SFPXCMP_MOD1_CC_GTE;
        break;
    case SFPXCMP_MOD1_CC_NE:
        new_op = SFPXCMP_MOD1_CC_EQ;
        break;
    case SFPXCMP_MOD1_CC_GTE:
        new_op = SFPXCMP_MOD1_CC_LT;
        break;
    case SFPXCMP_MOD1_CC_EQ:
        new_op = SFPXCMP_MOD1_CC_NE;
        break;
    case SFPXCMP_MOD1_CC_LTE:
        new_op = SFPXCMP_MOD1_CC_GT;
        break;
    case SFPXCMP_MOD1_CC_GT:
        new_op = SFPXCMP_MOD1_CC_LTE;
        break;
    }

    return (mod & ~SFPXCMP_MOD1_CC_MASK) | new_op;
}

inline bool SFPUConditional::issues_compc(bool negate) const
{
    int op = mod & SFPXCMP_MOD1_CC_MASK;
    return (!negate && op == SFPXCMP_MOD1_CC_LTE) || (negate && op == SFPXCMP_MOD1_CC_GT);
}

inline SFPUConditional::opType SFPUConditional::get_boolean_type(bool negate) const
{
    if (operation == opType::opOr) return negate ? opType::opAnd : opType::opOr;
    if (operation == opType::opAnd) return negate ? opType::opOr : opType::opAnd;
    throw;
}

int sfpu_rvtt_sfpxfcmps(const __rvtt_vec_t& v, unsigned int f, int mod)
{
    sfpu_conditionals.push_back(SFPUConditional(SFPUConditional::opType::opFcmpS, v, f, mod));
    return sfpu_conditionals.size() - 1;
}

int sfpu_rvtt_sfpxfcmpv(const __rvtt_vec_t& v1, const __rvtt_vec_t& v2, int mod)
{
    sfpu_conditionals.push_back(SFPUConditional(SFPUConditional::opType::opFcmpV, v1, v2, mod));
    return sfpu_conditionals.size() - 1;
}

int sfpu_rvtt_sfpxicmps(const __rvtt_vec_t& v, unsigned int i, int mod)
{
    sfpu_conditionals.push_back(SFPUConditional(SFPUConditional::opType::opIcmpS, v, i, mod));
    return sfpu_conditionals.size() - 1;
}

int sfpu_rvtt_sfpxicmpv(const __rvtt_vec_t& v1, const __rvtt_vec_t& v2, int mod)
{
    sfpu_conditionals.push_back(SFPUConditional(SFPUConditional::opType::opIcmpV, v1, v2, mod));
    return sfpu_conditionals.size() - 1;
}

int sfpu_rvtt_sfpxbool(int t, int a, int b)
{
    if (t == SFPXBOOL_MOD1_NOT) {
        sfpu_conditionals.back().negate();
    } else {
        SFPUConditional::opType op = (t == SFPXBOOL_MOD1_OR) ?
            SFPUConditional::opType::opOr : SFPUConditional::opType::opAnd;

        sfpu_conditionals.push_back(SFPUConditional(op, a, b));
    }

    return sfpu_conditionals.size() - 1;
}

void SFPUConditional::emit_conditional(int w, bool negate)
{
    const SFPUConditional& node = sfpu_conditionals[w];
    uint32_t mod = (negate ^ node.negated) ? node.negate_mod() : node.mod;
    
    switch (node.operation) {
    case SFPUConditional::opType::opFcmpS:
        sfpu_rvtt_sfpxscmp(node.v1, node.imm, mod);
        break;

    case SFPUConditional::opType::opFcmpV:
        sfpu_rvtt_sfpxvcmp(node.v1, node.v2, mod);
        break;

    case SFPUConditional::opType::opIcmpS:
        sfpu_rvtt_sfpxiadd_i(node.imm, node.v1, mod | SFPXIADD_MOD1_IS_SUB);
        break;

    case SFPUConditional::opType::opIcmpV:
        sfpu_rvtt_sfpxiadd_v(node.v1, node.v2, mod | SFPXIADD_MOD1_IS_SUB);
        break;

    case SFPUConditional::opType::opOr:
    case SFPUConditional::opType::opAnd:
        emit_boolean_tree(w, negate);
        break;

    default:
        fprintf(stderr, "Illegal operation in boolean tree: %d\n", static_cast<int>(sfpu_conditionals[w].operation));
        throw;
    }
}
