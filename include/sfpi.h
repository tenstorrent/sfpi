///////////////////////////////////////////////////////////////////////////////
// sfpi.h: SFPu Interface
//   This file provides a C++ wrapper around GCC style __builtin functions
//   which map to SFPU instructions.
//
// Note: Op refers to "Operation" while Operand is spelled out.  Usually.
//
// Local Register (LREG) Operations:
//   class VecUShort
//   class VecShort
//   class VecHalf
//
//   These classes are the workhorse classes of sfpi.  Operators are
//   overloaded to provide functionality such as:
//     VecHalf a, b, c, d;
//     VecHalf d = a * b + c;
//
//   Mathematical operations such as the above are supported out to the level
//   of a MAD so the above won't work if you add an extra operator at the
//   end.
//
//   The classes also support conditional operators for use in conjunction
//   with the CCCtrl class to enable predicated execution, for example:
//     VecHalf a;
//     p_if (a < 5.0F)
//   will load an immediate value of 5.0F into an LReg, perform the subtract
//   and test the result to set the CC.
//
//   Additional operations such as load immediate, extract/set exp, etc are
//   supported through class methods.
//
//
// Offset Operand:
//   constexpr OffsetOperand half;
//
//   There is a special operand called half to access Grayskull's ability to
//   bias math operations by 0.5F, used as:
//       d = a * b + c + half;
//   which will collapse down into a single SFPU instruction.  This will be
//   eliminated when gcc's combiner pass is used to create MADs and combine
//   instructions.
//
//
// Destination Register:
//   class DReg
//   constexpr RegFile<DReg, 64> dst_reg;
//
//   The Destination Register is modeled by a global variable which is
//   essentially an array of class Dreg.  DRegs provide much the same
//   functionality as LRegs, eg, the following is legal:
//       dst_reg[0] = dst_reg[1] * dst_reg[2] + dst_reg[3];
//   The above is expanded out to load local registers, perform the operation
//   and store back into the destination register.  Any missing functionality
//   (eg, there is no load immediate) accessed through LRegs.
//
//
// Constant Local Registers:
//   class CReg
//   constexpr CReg CReg_0p6928(5);
//
//   The constant value local registers are used in expressions by referencing
//   one of the names above and using them in mathematical operations such as:
//       a = b * c + CReg_0p6928;
//
// Predicated Execution:
//   class CCCtrl
//   macros p_if(), p_elseif(), p_else, p_endif
//
//   The class CCCtrl is used in conjunction with the LReg based classes to
//   enable predicated execution.  By convention the test infastructure indents
//   the code as if executing if/then/else in C++, for example:
//     CCCtrl cc;
//     VecHalf v = dst_reg[0];
//     cc.cc_if(v < 5.0F); {
//         // if side
//     } cc.cc_else(); {
//         // else side
//     }
//     cc.cc_endif();
//   The above chatter is reduced w/ use of macros listed above.  The macros
//   also balance {} to auto-destruct the CCCtrl when appropriate.
//
//  Boolean Operators:
//   && and || are handled by building a tree of operations and traversing
//   this at compile time, all with 0 run time overhead.  This requires some
//   compromises.  Ideally, this would be handled by an inline recursive
//   function, which gcc can do, but not with "always_inline".  Instead, the
//   emit() function uses a macro to expand out to 3 levels of recursiion.
//   This is ripe for moving into the compiler.
//
//  Internals:
//   The idea behind this wrapper is that it all collapses down to
//   typically one and sometimes more SFPU instructions at compile time
//   without any runtime overhead.  Many of the classes below are used to
//   track operations/operands as the compiler parses the code and then at a
//   trigger operation (eg, assignment) the operation gets expanded into an
//   instruction.  All of this working relies on compiling w/ optimization
//   enabled (-O) and that the vectors never get written to the stack.
//
//  Liveness:
//    A major complication of the SFPU for the compiler is that assigning a
//    variable doesn't always make the previous assignment obsolete.  For
//    example, assigning x, then executing a p_if and re-assigning x within
//    the predicate keeps x "alive" rather than letting it go dead prior to
//    the second assignment.  The solution for this is a hybrid
//    wrapper/compiler solution; a document exists which describes this in
//    detail.  The main piece for the wrapper is that assignments generate an
//    sfpassign_lv instruction which propogates the value from the previous
//    assignment, thought this can only be done if the variable was previously
//    written (initialized).
//
//  Ugliness:
//    The wrapper should get smaller over time as functionality is moved into
//    the compiler back end (optimizer) and front end (parser, eg, p_endif).
//    Currently the biggest uglinesses are:
//      p_endif
//      assignments return void so cannot be used in, eg conditionals
//        since the compiler doesn't understand predication, using assignments
//        in condtionals can result in evaluation of the assignments out of
//        order w/ regard to the predication CC bits
//      related to the above, add_cc, exexp_cc and lz_cc are exposed to handle
//        instructions within conditionals
//      && and || can only go 3 deep in one conditional
//      mads are complex
//      conditionals are complex
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <type_traits>
#include <sfpi_internal.h>
#include <sfpi_fp16.h>

namespace sfpi {

enum ExExpCC {
    ExExpCCLT0 = SFPEXEXP_MOD1_SET_CC_SGN_EXP,
    ExExpCCComp = SFPEXEXP_MOD1_SET_CC_COMP_EXP,
    ExExpCCGTE0 = SFPEXEXP_MOD1_SET_CC_SGN_COMP_EXP,
};
enum LzCC {
    LzCCNE0 = SFPLZ_MOD1_CC_NE0,
    LzCCEQ0 = SFPLZ_MOD1_CC_EQ0
};
enum IAddCC {
    IAddCCLT0 = SFPIADD_MOD1_CC_LT0,
    IAddCCGTE0 = SFPIADD_MOD1_CC_GTE0
};

//////////////////////////////////////////////////////////////////////////////
// Forward declarations
class CReg;
class Vec;
class VecHalf;
class VecShortBase;
class VecShort;
class VecUShort;
class VecCond;
class VecBool;
class CondComp;
class CondOpExExp;
class CondOpIAddI;
class CondOpIAddV;
class CondOpLz;
class CCCtrlBase;

//////////////////////////////////////////////////////////////////////////////
template<class Type, int N>
class RegFile {

public:
    constexpr Type operator[](const int x) const;
};

//////////////////////////////////////////////////////////////////////////////
class RegBase {
protected:
    int reg;

public:
    constexpr explicit RegBase(int r) : reg(r) {}
    constexpr int get() const { return reg; }
};

//////////////////////////////////////////////////////////////////////////////
class RegBaseInitializer {
    int n;

 public:
    constexpr RegBaseInitializer(int in) : n(in) {}
    constexpr int get() const { return n; }
};

//////////////////////////////////////////////////////////////////////////////
class DReg : public RegBase {
private:
    template<class Type, int N> friend class RegFile;
    constexpr explicit DReg(const RegBaseInitializer i) : RegBase(i.get() * SFP_DESTREG_STRIDE) {}

public:
    // Assign register to register
    template <typename vecType, typename std::enable_if_t<std::is_base_of<Vec, vecType>::value>* = nullptr>
    sfpi_inline void operator=(const vecType vec) const;
    sfpi_inline void operator=(const DReg dreg) const;
    sfpi_inline void operator=(const CReg creg) const;
    sfpi_inline void operator=(const ScalarFP16 f) const;
    sfpi_inline void operator=(const float f) const;
    sfpi_inline void operator=(const int32_t i) const;

    // Construct operator classes from operations
    sfpi_inline VecHalf operator+(const VecHalf b) const;
    sfpi_inline VecHalf operator-(const VecHalf b) const;
    sfpi_inline VecHalf operator-(const float f) const;
    sfpi_inline VecHalf operator-(const ScalarFP16 i) const;
    sfpi_inline VecHalf operator-() const;
    sfpi_inline VecHalf operator*(const VecHalf b) const;

    // Conditionals
    sfpi_inline CondComp operator==(const float x) const;
    sfpi_inline CondComp operator!=(const float x) const;
    sfpi_inline CondComp operator<(const float x) const;
    sfpi_inline CondComp operator>=(const float x) const;
    sfpi_inline CondComp operator==(const ScalarFP16 x) const;
    sfpi_inline CondComp operator!=(const ScalarFP16 x) const;
    sfpi_inline CondComp operator<(const ScalarFP16 x) const;
    sfpi_inline CondComp operator>=(const ScalarFP16 x) const;
    sfpi_inline CondComp operator==(const VecHalf x) const;
    sfpi_inline CondComp operator!=(const VecHalf x) const;
    sfpi_inline CondComp operator<(const VecHalf x) const;
    sfpi_inline CondComp operator>=(const VecHalf x) const;
};

//////////////////////////////////////////////////////////////////////////////
enum class LRegs {
    LReg0 = 0,
    LReg1 = 1,
    LReg2 = 2,
    LReg3 = 3,
    LRegCount = 4,
};

struct LRegAssignerInternal {
    const LRegs lreg;
    __rvtt_vec_t *v;
    LRegAssignerInternal(LRegs lr) : lreg(lr), v(nullptr) {}
};

class LRegAssigner {
    friend Vec;

private:
    LRegAssignerInternal lregs[4];

    sfpi_inline void assign(__rvtt_vec_t& in, LRegAssignerInternal& lr);

public:
    sfpi_inline LRegAssigner() : lregs{LRegs::LReg0, LRegs::LReg1, LRegs::LReg2, LRegs::LReg3} {}
    sfpi_inline ~LRegAssigner();
    sfpi_inline LRegAssignerInternal& assign(LRegs lr);
};

//////////////////////////////////////////////////////////////////////////////
class CReg : public RegBase {
public:
    constexpr explicit CReg(int r) : RegBase(r) {}

    // Construct operator classes from operations
    sfpi_inline VecHalf operator+(const VecHalf b) const;
    sfpi_inline VecHalf operator-(const VecHalf b) const;
    sfpi_inline VecHalf operator*(const VecHalf b) const;
    sfpi_inline CondComp operator==(const VecHalf x) const;
    sfpi_inline CondComp operator!=(const VecHalf x) const;
    sfpi_inline CondComp operator<(const VecHalf x) const;
    sfpi_inline CondComp operator>=(const VecHalf x) const;
};

//////////////////////////////////////////////////////////////////////////////
class Vec {
protected:
    bool initialized;
    __rvtt_vec_t v;

public:
    sfpi_inline Vec() : initialized(false) {}

    sfpi_inline void assign(const __rvtt_vec_t t);

    sfpi_inline __rvtt_vec_t get() const { return v; }
    sfpi_inline __rvtt_vec_t& get() { return v; }

    // Associate variable w/ a value pre-loaded into a particular lreg
    sfpi_inline void operator=(LRegAssignerInternal& lr);
};

//////////////////////////////////////////////////////////////////////////////
class VecHalf : public Vec {
private:
    sfpi_inline void loadi(const ScalarFP16 val);

public:
    VecHalf() = default;

    sfpi_inline VecHalf(const DReg dreg);
    sfpi_inline VecHalf(const CReg creg);
    sfpi_inline VecHalf(const ScalarFP16 f) { loadi(f); }
    sfpi_inline VecHalf(const float f) { loadi(f); }
    sfpi_inline VecHalf(const __rvtt_vec_t& t) { assign(t); }

    // Assignment
    sfpi_inline void operator=(const VecHalf in) { assign(in.v); }
    sfpi_inline void operator=(LRegAssignerInternal& lr) { Vec::operator=(lr); }

    // Construct operator from operations
    sfpi_inline VecHalf operator+(const VecHalf b) const;
    sfpi_inline void operator+=(const VecHalf);
    sfpi_inline VecHalf operator-(const VecHalf b) const;
    sfpi_inline VecHalf operator-(const float b) const;
    sfpi_inline VecHalf operator-(const ScalarFP16 b) const;
    sfpi_inline void operator-=(const VecHalf);
    sfpi_inline VecHalf operator-() const;
    sfpi_inline VecHalf operator*(const VecHalf b) const;
    sfpi_inline void operator*=(const VecHalf);

    // Conditionals
    sfpi_inline CondComp operator==(const float x) const;
    sfpi_inline CondComp operator==(const ScalarFP16 x) const;
    sfpi_inline CondComp operator==(const VecHalf x) const;
    sfpi_inline CondComp operator!=(const float x) const;
    sfpi_inline CondComp operator!=(const ScalarFP16 x) const;
    sfpi_inline CondComp operator!=(const VecHalf x) const;
    sfpi_inline CondComp operator<(const float x) const;
    sfpi_inline CondComp operator<(const ScalarFP16 x) const;
    sfpi_inline CondComp operator<(const VecHalf x) const;
    sfpi_inline CondComp operator>=(const float x) const;
    sfpi_inline CondComp operator>=(const ScalarFP16 x) const;
    sfpi_inline CondComp operator>=(const VecHalf x) const;
};

//////////////////////////////////////////////////////////////////////////////
class VecShortBase : public Vec {
 protected:
    sfpi_inline void loadsi(int32_t val);
    sfpi_inline void loadui(uint32_t val);

 public:
    template <typename vType, typename std::enable_if_t<std::is_base_of<Vec, vType>::value>* = nullptr>
    sfpi_inline explicit operator vType() const { return vType(v); }

    // Bit Operations
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline vType operator&(const vType b) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator&=(const vType b);
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline vType operator|(const vType b) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator|=(const vType b);
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline vType operator~() const;

    // Arith
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline vType add(int32_t val, unsigned int mod) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline vType operator+(const VecShortBase val) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline vType operator+(const CReg val) const;

    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline vType sub(int32_t val, unsigned int mod) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline vType operator-(const VecShortBase val) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline vType operator-(const CReg val) const;

    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void add_eq(int32_t val, unsigned int mod);
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator+=(const VecShortBase val);
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator+=(const CReg val);

    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void sub_eq(int32_t val, unsigned int mod);
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator-=(const VecShortBase val);
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator-=(const CReg val);

    // Shifts
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline vType operator<<(uint32_t amt) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator<<=(uint32_t amt);

    // Misc
    sfpi_inline const CondOpLz lz_cc(const Vec src, LzCC cc);
};

class VecShort : public VecShortBase {
    friend class VecUShort;

public:
    VecShort() = default;
    sfpi_inline VecShort(const __rvtt_vec_t& in) { assign(in); }
    sfpi_inline VecShort(const CReg creg) { v = __builtin_rvtt_sfpassignlr(creg.get()); initialized = true; }
    sfpi_inline VecShort(const VecShortBase in) { assign(in.get()); };
    sfpi_inline VecShort(short val) { loadsi(val); }
    sfpi_inline VecShort(int val) { loadsi(val); }
#ifndef __clang__
    sfpi_inline VecShort(int32_t val) { loadsi(val); }
#endif
    sfpi_inline VecShort(unsigned short val) { loadui(val); }
    sfpi_inline VecShort(unsigned int val) { loadui(val); }
#ifndef __clang__
    sfpi_inline VecShort(uint32_t val) { loadui(val); }
#endif

    // Assignment
    sfpi_inline void operator=(const VecShort in) { assign(in.v); }
    sfpi_inline void operator=(LRegAssignerInternal& lr) { Vec::operator=(lr); }

    // Operations
    sfpi_inline void operator&=(const VecShort b) { return this->VecShortBase::operator&=(b); }
    sfpi_inline void operator|=(const VecShort b) { return this->VecShortBase::operator|=(b); }
    sfpi_inline VecShort operator~() const { return this->VecShortBase::operator~<VecShort>(); }

    sfpi_inline VecShort operator+(int32_t val) const { return this->VecShortBase::add<VecShort>(val, SFPIADD_I_EX_MOD1_SIGNED); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline VecShort operator+(const vType val) const { return this->VecShortBase::operator+<VecShort>(val); }
    sfpi_inline VecShort operator+(const CReg val) const { return this->VecShortBase::operator+<VecShort>(val); }

    sfpi_inline VecShort operator-(int32_t val) const { return this->VecShortBase::sub<VecShort>(val, SFPIADD_I_EX_MOD1_SIGNED); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline VecShort operator-(const vType val) const { return this->VecShortBase::operator-<VecShort>(val); }
    sfpi_inline VecShort operator-(const CReg val) const { return this->VecShortBase::operator-<VecShort>(val); }

    sfpi_inline void operator+=(int32_t val) { this->VecShortBase::add_eq<VecShort>(val, SFPIADD_I_EX_MOD1_SIGNED); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator+=(const vType val) { this->VecShortBase::operator+=<VecShort>(val); }
    sfpi_inline void operator+=(const CReg val) { this->VecShortBase::operator-=<VecShort>(val); }

    sfpi_inline void operator-=(int32_t val) { this->VecShortBase::sub_eq<VecShort>(val, SFPIADD_I_EX_MOD1_SIGNED); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator-=(const vType val) { this->VecShortBase::operator-=<VecShort>(val); }
    sfpi_inline void operator-=(const CReg val) { this->VecShortBase::operator-=<VecShort>(val); }

    sfpi_inline VecShort operator<<(uint32_t amt) const { return this->VecShortBase::operator<<<VecShort>(amt); }
    sfpi_inline void operator<<=(uint32_t amt) { return this->VecShortBase::operator<<=<VecShort>(amt); }

    // Conditionals
    sfpi_inline const CondComp operator==(int32_t val) const;
    sfpi_inline const CondComp operator!=(int32_t val) const;
    sfpi_inline const CondComp operator<(int32_t val) const;
    sfpi_inline const CondComp operator>=(int32_t val) const;
    sfpi_inline const CondComp operator<(const VecShortBase src) const;
    sfpi_inline const CondComp operator>=(const VecShortBase src) const;

    sfpi_inline const CondOpExExp exexp_cc(VecHalf src, const ExExpCC cc);
    sfpi_inline const CondOpExExp exexp_nodebias_cc(VecHalf src, const ExExpCC cc);
    sfpi_inline const CondOpLz lz_cc(const Vec src, LzCC cc);
    sfpi_inline const CondOpIAddI add_cc(const VecShort src, int32_t val, IAddCC cc);
    sfpi_inline const CondOpIAddV add_cc(const VecShort src, IAddCC cc);
};

//////////////////////////////////////////////////////////////////////////////
class VecUShort : public VecShortBase {
    friend class VecShort;

private:

public:
    VecUShort() = default;
    sfpi_inline VecUShort(const __rvtt_vec_t& in) { assign(in); }
    sfpi_inline VecUShort(const CReg creg) { v = __builtin_rvtt_sfpassignlr(creg.get()); initialized = true; }
    sfpi_inline VecUShort(const VecShortBase in) { assign(in.get()); }
    sfpi_inline VecUShort(short val) { loadsi(val); }
    sfpi_inline VecUShort(int val) { loadsi(val); }
#ifndef __clang__
    sfpi_inline VecUShort(int32_t val) { loadsi(val); }
#endif
    sfpi_inline VecUShort(unsigned short val) { loadui(val); }
    sfpi_inline VecUShort(unsigned int val) { loadui(val); }
#ifndef __clang__
    sfpi_inline VecUShort(uint32_t val) { loadui(val); }
#endif

    // Assignment
    sfpi_inline void operator=(const VecUShort in ) { assign(in.v); }
    sfpi_inline void operator=(LRegAssignerInternal& lr) { Vec::operator=(lr); }

    // Operations
    sfpi_inline void operator&=(const VecUShort b) { return this->VecShortBase::operator&=(b); }
    sfpi_inline void operator|=(const VecUShort b) { return this->VecShortBase::operator|=(b); }
    sfpi_inline VecUShort operator~() const { return this->VecShortBase::operator~<VecUShort>(); }

    sfpi_inline VecUShort operator+(int32_t val) const { return this->VecShortBase::add<VecUShort>(val, 0); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline VecUShort operator+(const vType val) const { return this->VecShortBase::operator+<VecUShort>(val); }
    sfpi_inline VecUShort operator+(const CReg val) const { return this->VecShortBase::operator+<VecUShort>(val); }

    sfpi_inline VecUShort operator-(int32_t val) const { return this->VecShortBase::sub<VecUShort>(val, 0); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline VecUShort operator-(const vType val) const { return this->VecShortBase::operator-<VecUShort>(val); }
    sfpi_inline VecUShort operator-(const CReg val) const { return this->VecShortBase::operator+<VecUShort>(val); }

    sfpi_inline void operator+=(int32_t val) { this->VecShortBase::add_eq<VecUShort>(val, 0); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator+=(const vType val) { this->VecShortBase::operator+=<VecUShort>(val); }
    sfpi_inline void operator+=(const CReg val) { this->VecShortBase::operator+=<VecUShort>(val); }

    sfpi_inline void operator-=(int32_t val) { this->VecShortBase::sub_eq<VecUShort>(val, 0); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>* = nullptr>
    sfpi_inline void operator-=(const vType val) { this->VecShortBase::operator-=<VecUShort>(val); }
    sfpi_inline void operator-=(const CReg val) { this->VecShortBase::operator-=<VecUShort>(val); }

    sfpi_inline VecUShort operator<<(uint32_t amt) const { return this->VecShortBase::operator<<<VecUShort>(amt); }
    sfpi_inline VecUShort operator>>(uint32_t amt) const;
    sfpi_inline void operator<<=(uint32_t amt) { this->VecShortBase::operator<<=<VecUShort>(amt); }
    sfpi_inline void operator>>=(uint32_t amt);

    // Conditionals
    sfpi_inline const CondComp operator==(int32_t val) const;
    sfpi_inline const CondComp operator!=(int32_t val) const;
    sfpi_inline const CondComp operator<(int32_t val) const;
    sfpi_inline const CondComp operator>=(int32_t val) const;
    sfpi_inline const CondComp operator<(const VecShortBase src) const;
    sfpi_inline const CondComp operator>=(const VecShortBase src) const;

    sfpi_inline const CondOpIAddI add_cc(const VecUShort src, int32_t val, IAddCC cc);
    sfpi_inline const CondOpIAddV add_cc(const VecUShort src, IAddCC cc);
};

//////////////////////////////////////////////////////////////////////////////
class Operand {
public:
    enum class Type {
        Null,
        Vec,
        VecPtr,
        ScalarFP16,
    };

private:
    const Type kind;

    union {
        // Some __builtins require the dst LREG as an Operand, can't be const
        // Ripe for const abuse
        const void* const null_ptr;
        const Vec vec;
        Vec* const vec_ptr;
        const uint32_t uint_val;
        const ScalarFP16 scalarfp_val;
    };

    const bool negate;

public:
    sfpi_inline Operand() : kind(Type::Null), null_ptr(nullptr), negate(false) {}
    sfpi_inline Operand(const Vec v, bool neg = false) : kind(Type::Vec), vec(v), negate(neg) {}
    sfpi_inline Operand(Vec* const v, bool neg = false) : kind(Type::VecPtr), vec_ptr(v), negate(neg) {}
    sfpi_inline Operand(const ScalarFP16 v) : kind(Type::ScalarFP16), scalarfp_val(v), negate(false) {}

    sfpi_inline Type get_type() const { return kind; }
    sfpi_inline bool is_neg() const { return negate; }

    sfpi_inline const Vec get_vec() const { return vec; }
    sfpi_inline Vec* const get_vec_ptr() const { return vec_ptr; }
    sfpi_inline const ScalarFP16 get_scalarfp() const { return scalarfp_val; }
};

//////////////////////////////////////////////////////////////////////////////
// Handle conditionals.  Leaves are VecCond, VecBool.
class CondOperand {
public:
    enum class Type {
        VecCond,
        VecBool,
    };

private:
    const Type type;
    union {
        const VecCond* const op;
        const VecBool* const cond;
    };

public:
    sfpi_inline CondOperand(const VecCond& a) : type(Type::VecCond), op(&a) {}
    sfpi_inline CondOperand(const VecBool& a) : type(Type::VecBool), cond(&a) {}

    sfpi_inline Type get_type() const { return type; }
    sfpi_inline const VecBool& get_cond() const { return *cond; }
    sfpi_inline const VecCond& get_op() const { return *op; }

    sfpi_inline void emit() const;
};

//////////////////////////////////////////////////////////////////////////////
// Build a tree of conditionals as the compiler parses them
// Traverse the tree at emit time to generate the code
class VecBool {
public:
    enum class VecBoolType {
        Or,
        And,
    };

private:
    const VecBoolType type;
    CondOperand op_a;
    CondOperand op_b;
    bool negate;

public:
    sfpi_inline VecBool(VecBoolType t, const VecCond& a, const VecCond& b) : type(t), op_a(a), op_b(b), negate(false) {}
    sfpi_inline VecBool(VecBoolType t, const VecCond& a, const VecBool& b) : type(t), op_a(a), op_b(b), negate(false) {}
    sfpi_inline VecBool(VecBoolType t, const VecBool& a, const VecCond& b) : type(t), op_a(a), op_b(b), negate(false) {}
    sfpi_inline VecBool(VecBoolType t, const VecBool& a, const VecBool& b) : type(t), op_a(a), op_b(b), negate(false) {}

    sfpi_inline const VecBool operator!() const;

    sfpi_inline const VecBool operator&&(const VecCond& b) const { return VecBool(VecBoolType::And, *this, b); }
    sfpi_inline const VecBool operator||(const VecCond& b) const { return VecBool(VecBoolType::Or, *this, b); }
    sfpi_inline const VecBool operator&&(const VecBool& b) const { return VecBool(VecBoolType::And, *this, b); }
    sfpi_inline const VecBool operator||(const VecBool& b) const { return VecBool(VecBoolType::Or, *this, b); }

    sfpi_inline VecBoolType get_type() const { return type; }
    sfpi_inline VecBoolType get_neg_type() const { return (type == VecBoolType::Or) ? VecBoolType::And : VecBoolType::Or; }
    sfpi_inline const CondOperand& get_op_a() const { return op_a; }
    sfpi_inline const CondOperand& get_op_b() const { return op_b; }

    sfpi_inline void emit(bool negate = false) const;
};

//////////////////////////////////////////////////////////////////////////////
class VecCond {
protected:
    enum class CondOpType {
        CompareFloat,
        CompareVecHalf,
        CompareShort,
        CompareVecShort,
        ExExp,
        Lz,
        IAddI,
        IAddV
    };

private:
    const CondOpType type;
    const Operand op_a;
    Operand op_b;
    const int32_t imm;
    uint32_t mod1;
    uint32_t neg_mod1;

public:
    sfpi_inline VecCond(CondOpType t) : type(t), op_a(), op_b(), imm(0), mod1(0), neg_mod1(0) {}

    template <class typeA, class typeB>
    sfpi_inline VecCond(CondOpType t, const typeA a, const typeB b, int32_t i, uint32_t m, uint32_t nm) : type(t), op_a(a), op_b(b), imm(i), mod1(m), neg_mod1(nm) {}

    template <class typeA>
    sfpi_inline VecCond(CondOpType t, const typeA a, Vec* const b, int32_t i, uint32_t m, uint32_t nm) : type(t), op_a(a), op_b(b), imm(i), mod1(m), neg_mod1(nm) {}

    template <class typeA>
    sfpi_inline VecCond(CondOpType t, const typeA a, int32_t i, uint32_t m, uint32_t nm) : type(t), op_a(a), op_b(), imm(i), mod1(m), neg_mod1(nm) {}

    template <class typeA>
    sfpi_inline VecCond(CondOpType t, const typeA a, VecShortBase b, uint32_t m, uint32_t nm) : type(t), op_a(a), op_b(b), imm(0), mod1(m), neg_mod1(nm) {}

    template <class typeA>
    sfpi_inline VecCond(CondOpType t, const typeA a, const ScalarFP16 b, int32_t i, uint32_t m, uint32_t nm) : type(t), op_a(a), op_b(b), imm(i), mod1(m), neg_mod1(nm) {}

    sfpi_inline VecCond(CondOpType t, const VecShort a, int32_t i, uint32_t m, uint32_t nm);

    // Negate
    sfpi_inline const VecCond operator!() const;

    // Create boolean operations from conditional operations
    sfpi_inline const VecBool operator&&(const VecCond& b) const { return VecBool(VecBool::VecBoolType::And, *this, b); }
    sfpi_inline const VecBool operator||(const VecCond& b) const { return VecBool(VecBool::VecBoolType::Or, *this, b); }
    sfpi_inline const VecBool operator&&(const VecBool& b) const { return VecBool(VecBool::VecBoolType::And, *this, b); }
    sfpi_inline const VecBool operator||(const VecBool& b) const { return VecBool(VecBool::VecBoolType::Or, *this, b); }

    sfpi_inline void emit(bool negate) const;
};

//////////////////////////////////////////////////////////////////////////////
// Conditional Comparison
class CondComp : public VecCond {
public:
    enum CondCompOpType {
        CompLT0 = SFPCMP_EX_MOD1_CC_LT0,
        CompNE0 = SFPCMP_EX_MOD1_CC_NE0,
        CompGTE0 = SFPCMP_EX_MOD1_CC_GTE0,
        CompEQ0 = SFPCMP_EX_MOD1_CC_EQ0,
    };

private:
    sfpi_inline CondCompOpType not_cond(const CondCompOpType t) const;

public:
    template <class type>
    sfpi_inline CondComp(const CondCompOpType t, const type a, const ScalarFP16 b) : VecCond(CondOpType::CompareFloat, a, b, 0, t, not_cond(t)) {}

    sfpi_inline CondComp(const CondCompOpType t, const VecHalf a, const VecHalf b) : VecCond(CondOpType::CompareVecHalf, a, b, 0, t, not_cond(t)) {}

    template <class type>
    sfpi_inline CondComp(const CondCompOpType t, const type a, const int32_t b, const unsigned int mod) : VecCond(CondOpType::CompareShort, a, b, t | mod, not_cond(t) | mod) {}

    template <class type>
    sfpi_inline CondComp(const CondCompOpType t, const type a, const VecShortBase b, const unsigned int mod) : VecCond(CondOpType::CompareVecShort, a, b, t | mod, not_cond(t) | mod) {}
};

//////////////////////////////////////////////////////////////////////////////
// Conditional Operations

class CondOpExExp : public VecCond {
    sfpi_inline ExExpCC not_cond(ExExpCC cc) const;

public:
    sfpi_inline CondOpExExp(Vec* const d, const Vec s, unsigned short debias, ExExpCC cc) : VecCond(CondOpType::ExExp, s, d, 0, debias | cc, debias | not_cond(cc)) {}
};

class CondOpLz : public VecCond {
    sfpi_inline LzCC not_cond(LzCC cc) const;

public:
    sfpi_inline CondOpLz(Vec* const d, const Vec s, LzCC cc) : VecCond(CondOpType::Lz, s, d, 0, cc, not_cond(cc)) {}
};

class CondOpIAddI : public VecCond {
    sfpi_inline IAddCC not_cond(IAddCC cc) const;

public:
    sfpi_inline CondOpIAddI(Vec* const d, const Vec s, IAddCC cc, int32_t imm) : VecCond(CondOpType::IAddI, s, d, imm, cc | SFPIADD_MOD1_ARG_IMM, not_cond(cc) | SFPIADD_MOD1_ARG_IMM) {}
};

class CondOpIAddV : public VecCond {
    sfpi_inline IAddCC not_cond(IAddCC cc) const;

public:
    sfpi_inline CondOpIAddV(Vec* d, const Vec s, IAddCC cc) : VecCond(CondOpType::IAddV, s, d, 0, cc, not_cond(cc)) {}
};

//////////////////////////////////////////////////////////////////////////////
class CCCtrl {
protected:
    int push_count;

    sfpi_inline void pop();

public:
    sfpi_inline CCCtrl();
    sfpi_inline ~CCCtrl();

    sfpi_inline void cc_if(const VecBool& op) const;
    sfpi_inline void cc_if(const VecCond& op) const;
    sfpi_inline void cc_else() const;
    sfpi_inline void cc_elseif(const VecBool& cond);
    sfpi_inline void cc_elseif(const VecCond& cond);

    sfpi_inline void push();

    static sfpi_inline void enable_cc();
};

//////////////////////////////////////////////////////////////////////////////
constexpr CReg CReg_0(CREG_IDX_0);
constexpr CReg CReg_0p6929(CREG_IDX_0P692871094);
constexpr CReg CReg_Neg_1p0068(CREG_IDX_NEG_1P00683594);
constexpr CReg CReg_1p4424(CREG_IDX_1P442382813);
constexpr CReg CReg_0p8369(CREG_IDX_0P836914063);
constexpr CReg CReg_Neg_0p5(CREG_IDX_NEG_0P5);
constexpr CReg CReg_1(CREG_IDX_1);
constexpr CReg CReg_Neg_1(CREG_IDX_NEG_1);
constexpr CReg CReg_0p0020(CREG_IDX_0P001953125);
constexpr CReg CReg_Neg_0p6748(CREG_IDX_NEG_0P67480469);
constexpr CReg CReg_Neg_0p3447(CREG_IDX_NEG_0P34472656);
constexpr CReg CReg_TileId(CREG_IDX_TILEID);

constexpr RegFile<DReg, 64> dst_reg;

//////////////////////////////////////////////////////////////////////////////
namespace sfpi_int {

sfpi_inline VecHalf fp_add(const VecHalf a, const VecHalf b)
{
    return __builtin_rvtt_sfpadd(a.get(), b.get(), 0);
}

sfpi_inline VecHalf fp_sub(const VecHalf a, const VecHalf b)
{
    __rvtt_vec_t tmp = __builtin_rvtt_sfpmov(b.get(), SFPMOV_MOD1_COMPSIGN);
    return __builtin_rvtt_sfpadd(a.get(), tmp, 0);
}

sfpi_inline VecHalf fp_mul(const VecHalf a, const VecHalf b)
{
    return __builtin_rvtt_sfpmul(a.get(), b.get(), 0);
}

}

//////////////////////////////////////////////////////////////////////////////
template<class TYPE, int N>
constexpr TYPE RegFile<TYPE, N>::operator[](const int x) const {
    return TYPE(RegBaseInitializer(x));
}

sfpi_inline VecHalf DReg::operator+(const VecHalf b) const {return sfpi_int::fp_add(VecHalf(*this), b); }
sfpi_inline VecHalf DReg::operator-(const VecHalf b) const { return sfpi_int::fp_sub(VecHalf(*this), b); }
sfpi_inline VecHalf DReg::operator-(const float b) const { return sfpi_int::fp_add(VecHalf(*this), VecHalf(-b)); }
sfpi_inline VecHalf DReg::operator-(const ScalarFP16 b) const { return sfpi_int::fp_add(VecHalf(*this), b.negate()); }
sfpi_inline VecHalf DReg::operator*(const VecHalf b) const  { return sfpi_int::fp_mul(VecHalf(*this), b); }
sfpi_inline CondComp DReg::operator==(const float x) const {return CondComp(CondComp::CompEQ0, VecHalf(*this), ScalarFP16(x)); }
sfpi_inline CondComp DReg::operator!=(const float x) const { return CondComp(CondComp::CompNE0, VecHalf(*this), ScalarFP16(x)); }
sfpi_inline CondComp DReg::operator<(const float x) const { return CondComp(CondComp::CompLT0, VecHalf(*this), ScalarFP16(x)); }
sfpi_inline CondComp DReg::operator>=(const float x) const { return CondComp(CondComp::CompGTE0, VecHalf(*this), ScalarFP16(x)); }
sfpi_inline CondComp DReg::operator==(const ScalarFP16 x) const {return CondComp(CondComp::CompEQ0, VecHalf(*this), x); }
sfpi_inline CondComp DReg::operator!=(const ScalarFP16 x) const { return CondComp(CondComp::CompNE0, VecHalf(*this), x); }
sfpi_inline CondComp DReg::operator<(const ScalarFP16 x) const { return CondComp(CondComp::CompLT0, VecHalf(*this), x); }
sfpi_inline CondComp DReg::operator>=(const ScalarFP16 x) const { return CondComp(CondComp::CompGTE0, VecHalf(*this), x); }
sfpi_inline CondComp DReg::operator==(const VecHalf x) const {return CondComp(CondComp::CompEQ0, VecHalf(*this), x); }
sfpi_inline CondComp DReg::operator!=(const VecHalf x) const { return CondComp(CondComp::CompNE0, VecHalf(*this), x); }
sfpi_inline CondComp DReg::operator<(const VecHalf x) const { return CondComp(CondComp::CompLT0, VecHalf(*this), x); }
sfpi_inline CondComp DReg::operator>=(const VecHalf x) const { return CondComp(CondComp::CompGTE0, VecHalf(*this), x); }

template <>
sfpi_inline void DReg::operator=(const VecHalf vec) const
{
    // For  half, rebias
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

template <typename vecType, typename std::enable_if_t<std::is_base_of<Vec, vecType>::value>*>
sfpi_inline void DReg::operator=(const vecType vec) const
{
    // For short/ushort, do not rebias
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_INT, reg);
}

sfpi_inline void DReg::operator=(const DReg dreg) const
{
    VecHalf tmp = dreg;
    __builtin_rvtt_sfpstore(tmp.get(), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

sfpi_inline void DReg::operator=(const CReg creg) const
{
    __rvtt_vec_t lr = __builtin_rvtt_sfpassignlr(creg.get());
    __builtin_rvtt_sfpstore(lr, SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

sfpi_inline void DReg::operator=(ScalarFP16 f) const
{
    VecHalf v(f);
    *this = v;
}

sfpi_inline void DReg::operator=(float f) const
{
    operator=(ScalarFP16(f));
}

sfpi_inline void DReg::operator=(int32_t i) const
{
    VecShort v = i;
    *this = v;
}

sfpi_inline VecHalf DReg::operator-() const
{
    VecHalf tmp = *this;
    return __builtin_rvtt_sfpmov(tmp.get(), SFPMOV_MOD1_COMPSIGN);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline VecHalf CReg::operator+(const VecHalf b) const { return sfpi_int::fp_add(VecHalf(*this), b); }
sfpi_inline VecHalf CReg::operator-(const VecHalf b) const { return sfpi_int::fp_sub(VecHalf(*this), b); }
sfpi_inline VecHalf CReg::operator*(const VecHalf b) const { return sfpi_int::fp_mul(VecHalf(*this), b); }
sfpi_inline CondComp CReg::operator==(const VecHalf x) const { return CondComp(CondComp::CompEQ0, *this, x); }
sfpi_inline CondComp CReg::operator!=(const VecHalf x) const { return CondComp(CondComp::CompNE0, *this, x); }
sfpi_inline CondComp CReg::operator<(const VecHalf x) const { return CondComp(CondComp::CompLT0, *this, x); }
sfpi_inline CondComp CReg::operator>=(const VecHalf x) const { return CondComp(CondComp::CompGTE0, *this, x); }

//////////////////////////////////////////////////////////////////////////////
sfpi_inline LRegAssigner::~LRegAssigner()
{
    // Explict as the optimizer can't handle a loop
    if (lregs[0].v != nullptr) {
        __builtin_rvtt_sfpkeepalive(*lregs[0].v, 0);
    }
    if (lregs[1].v != nullptr) {
        __builtin_rvtt_sfpkeepalive(*lregs[1].v, 1);
    }
    if (lregs[2].v != nullptr) {
        __builtin_rvtt_sfpkeepalive(*lregs[2].v, 2);
    }
    if (lregs[3].v != nullptr) {
        __builtin_rvtt_sfpkeepalive(*lregs[3].v, 3);
    }
}

sfpi_inline void LRegAssigner::assign(__rvtt_vec_t& in, LRegAssignerInternal& lr)
{
    lr.v = &in;
}

sfpi_inline LRegAssignerInternal& LRegAssigner::assign(LRegs lr)
{
    return lregs[static_cast<std::underlying_type<LRegs>::type>(lr)];
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void Vec::assign(const __rvtt_vec_t in)
{
    v = (initialized) ? __builtin_rvtt_sfpassign_lv(v, in) : in;
    initialized = true;
}

sfpi_inline void Vec::operator=(LRegAssignerInternal& lr)
{
    v = __builtin_rvtt_sfpassignlr(static_cast<std::underlying_type<LRegs>::type>(lr.lreg));
    lr.v = &v;
    initialized = true;
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline VecHalf VecHalf::operator+(const VecHalf b) const { return sfpi_int::fp_add(*this, b); }
sfpi_inline VecHalf VecHalf::operator-(const VecHalf b) const { return sfpi_int::fp_sub(*this, b); }
sfpi_inline VecHalf VecHalf::operator-(const float b) const { return sfpi_int::fp_add(*this, VecHalf(-b)); }
sfpi_inline VecHalf VecHalf::operator-(const ScalarFP16 b) const { return sfpi_int::fp_add(*this, b.negate()); }
sfpi_inline VecHalf VecHalf::operator*(const VecHalf b) const { return sfpi_int::fp_mul(*this, b); }
sfpi_inline CondComp VecHalf::operator==(const float x) const { return CondComp(CondComp::CompEQ0, *this, ScalarFP16(x)); }
sfpi_inline CondComp VecHalf::operator!=(const float x) const { return CondComp(CondComp::CompNE0, *this, ScalarFP16(x)); }
sfpi_inline CondComp VecHalf::operator<(const float x) const { return CondComp(CondComp::CompLT0, *this, ScalarFP16(x)); }
sfpi_inline CondComp VecHalf::operator>=(const float x) const { return CondComp(CondComp::CompGTE0, *this, ScalarFP16(x)); }
sfpi_inline CondComp VecHalf::operator==(const ScalarFP16 x) const { return CondComp(CondComp::CompEQ0, *this, x); }
sfpi_inline CondComp VecHalf::operator!=(const ScalarFP16 x) const { return CondComp(CondComp::CompNE0, *this, x); }
sfpi_inline CondComp VecHalf::operator<(const ScalarFP16 x) const { return CondComp(CondComp::CompLT0, *this, x); }
sfpi_inline CondComp VecHalf::operator>=(const ScalarFP16 x) const { return CondComp(CondComp::CompGTE0, *this, x); }
sfpi_inline CondComp VecHalf::operator==(const VecHalf x) const { return CondComp(CondComp::CompEQ0, *this, x); }
sfpi_inline CondComp VecHalf::operator!=(const VecHalf x) const { return CondComp(CondComp::CompNE0, *this, x); }
sfpi_inline CondComp VecHalf::operator<(const VecHalf x) const { return CondComp(CondComp::CompLT0, *this, x); }
sfpi_inline CondComp VecHalf::operator>=(const VecHalf x) const { return CondComp(CondComp::CompGTE0, *this, x); }

sfpi_inline void VecHalf::operator*=(const VecHalf m)
{
    v = __builtin_rvtt_sfpmul(v, m.get(), SFPMAD_MOD1_OFFSET_NONE);
}

sfpi_inline void VecHalf::operator+=(const VecHalf a)
{
    v = __builtin_rvtt_sfpadd(v, a.get(), SFPMAD_MOD1_OFFSET_NONE);
}

sfpi_inline void VecHalf::operator-=(const VecHalf a)
{
    v = __builtin_rvtt_sfpadd(v, (-a).get(), SFPMAD_MOD1_OFFSET_NONE);
}

sfpi_inline VecHalf::VecHalf(const DReg dreg)
{
    v = __builtin_rvtt_sfpload(SFPLOAD_MOD0_REBIAS_EXP, dreg.get());
    initialized = true;
}

sfpi_inline VecHalf::VecHalf(const CReg creg)
{
    v = __builtin_rvtt_sfpassignlr(creg.get());
    initialized = true;
}

sfpi_inline VecHalf VecHalf::operator-() const
{
    return __builtin_rvtt_sfpmov(v, SFPMOV_MOD1_COMPSIGN);
}

sfpi_inline void VecHalf::loadi(const ScalarFP16 val)
{
    v = __builtin_rvtt_sfploadi(val.get_format(), val.get());
    initialized = true;
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void VecShortBase::loadsi(int32_t val)
{
    v = __builtin_rvtt_sfploadi(SFPLOADI_MOD0_SHORT, val);
    initialized = true;
}

sfpi_inline void VecShortBase::loadui(uint32_t val)
{
    v = __builtin_rvtt_sfploadi(SFPLOADI_MOD0_USHORT, val);
    initialized = true;
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline vType VecShortBase::operator&(const vType b) const
{
    return __builtin_rvtt_sfpand(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline void VecShortBase::operator&=(const vType b)
{
    v = __builtin_rvtt_sfpand(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline vType VecShortBase::operator|(const vType b) const
{
    return __builtin_rvtt_sfpor(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline void VecShortBase::operator|=(const vType b)
{
    v = __builtin_rvtt_sfpor(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline vType VecShortBase::operator~() const
{
    return __builtin_rvtt_sfpnot(v);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline vType VecShortBase::operator<<(uint32_t amt) const
{
    return __builtin_rvtt_sfpshft_i(v, amt);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline void VecShortBase::operator<<=(uint32_t amt)
{
    v = (static_cast<vType>(*this) << amt).get();
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline vType VecShortBase::add(int32_t val, unsigned int mod_base) const
{
    return __builtin_rvtt_sfpiadd_i_ex(v, val, mod_base);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline vType VecShortBase::operator+(const VecShortBase val) const
{
    return __builtin_rvtt_sfpiadd_v_ex(val.get(), v, 0);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline vType VecShortBase::operator+(const CReg val) const
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    return __builtin_rvtt_sfpiadd_v_ex(c, v, 0);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline vType VecShortBase::sub(int32_t val, unsigned int mod_base) const
{
    return __builtin_rvtt_sfpiadd_i_ex(v, val, mod_base | SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline vType VecShortBase::operator-(const VecShortBase val) const
{
    return __builtin_rvtt_sfpiadd_v_ex(val.get(), v, SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline vType VecShortBase::operator-(const CReg val) const
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    return __builtin_rvtt_sfpiadd_v_ex(c, v, SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline void VecShortBase::add_eq(int32_t val, unsigned int mod_base)
{
    v = __builtin_rvtt_sfpiadd_i_ex(v, val, mod_base);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline void VecShortBase::operator+=(const VecShortBase val)
{
    v = __builtin_rvtt_sfpiadd_v_ex(v, val.get(), 0);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline void VecShortBase::operator+=(const CReg val)
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    v = __builtin_rvtt_sfpiadd_v_ex(c, v, 0);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline void VecShortBase::sub_eq(int32_t val, unsigned int mod_base)
{
    v = __builtin_rvtt_sfpiadd_i_ex(v, val, mod_base | SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline void VecShortBase::operator-=(const VecShortBase val)
{
    v = __builtin_rvtt_sfpiadd_v_ex(val.get(), v, SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<VecShortBase, vType>::value>*>
sfpi_inline void VecShortBase::operator-=(const CReg val)
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    v = __builtin_rvtt_sfpiadd_v_ex(c, v, SFPIADD_EX_MOD1_IS_SUB);
}

sfpi_inline const CondOpLz VecShortBase::lz_cc(const Vec src, LzCC cc)
{
    return CondOpLz(this, src, cc);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline const CondOpExExp VecShort::exexp_cc(VecHalf src, const ExExpCC cc) { return CondOpExExp(this, src, SFPEXEXP_MOD1_DEBIAS, cc); }
sfpi_inline const CondOpExExp VecShort::exexp_nodebias_cc(VecHalf src, const ExExpCC cc) { return CondOpExExp(this, src, SFPEXEXP_MOD1_NODEBIAS, cc); }
sfpi_inline const CondOpLz VecShort::lz_cc(const Vec src, LzCC cc) { return CondOpLz(this, src, cc); }
sfpi_inline const CondOpIAddI VecShort::add_cc(const VecShort src, int32_t val, IAddCC cc) { return CondOpIAddI(this, src, cc, val); }
sfpi_inline const CondOpIAddV VecShort::add_cc(const VecShort src, IAddCC cc) { return CondOpIAddV(this, src, cc); }

sfpi_inline const CondComp VecShort::operator==(int32_t val) const { return CondComp(CondComp::CompEQ0, *this, val, SFPIADD_I_EX_MOD1_SIGNED); }
sfpi_inline const CondComp VecShort::operator!=(int32_t val) const { return CondComp(CondComp::CompNE0, *this, val, SFPIADD_I_EX_MOD1_SIGNED); }
sfpi_inline const CondComp VecShort::operator<(int32_t val) const { return CondComp(CondComp::CompLT0, *this, val, SFPIADD_I_EX_MOD1_SIGNED); }
sfpi_inline const CondComp VecShort::operator>=(int32_t val) const { return CondComp(CondComp::CompGTE0, *this, val, SFPIADD_I_EX_MOD1_SIGNED); }

sfpi_inline const CondComp VecShort::operator<(const VecShortBase src) const { return CondComp(CondComp::CompLT0, *this, src, 0); }
sfpi_inline const CondComp VecShort::operator>=(const VecShortBase src) const { return CondComp(CondComp::CompGTE0, *this, src, 0); }

//////////////////////////////////////////////////////////////////////////////
sfpi_inline const CondOpIAddI VecUShort::add_cc(const VecUShort src, int32_t val, IAddCC cc) { return CondOpIAddI(this, src, cc, val); }
sfpi_inline const CondOpIAddV VecUShort::add_cc(const VecUShort src, IAddCC cc) { return CondOpIAddV(this, src, cc); }

sfpi_inline const CondComp VecUShort::operator==(int32_t val) const { return CondComp(CondComp::CompEQ0, *this, val, 0); }
sfpi_inline const CondComp VecUShort::operator!=(int32_t val) const { return CondComp(CondComp::CompNE0, *this, val, 0); }
sfpi_inline const CondComp VecUShort::operator<(int32_t val) const { return CondComp(CondComp::CompLT0, *this, val, 0); }
sfpi_inline const CondComp VecUShort::operator>=(int32_t val) const { return CondComp(CondComp::CompGTE0, *this, val, 0); }

sfpi_inline const CondComp VecUShort::operator<(const VecShortBase src) const { return CondComp(CondComp::CompLT0, *this, src, 0); }
sfpi_inline const CondComp VecUShort::operator>=(const VecShortBase src) const { return CondComp(CondComp::CompGTE0, *this, src, 0); }

sfpi_inline VecUShort VecUShort::operator>>(uint32_t amt) const
{
    return __builtin_rvtt_sfpshft_i(v, -amt);
}

sfpi_inline void VecUShort::operator>>=(uint32_t amt)
{
    v = (*this >> amt).get();
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void CondOperand::emit() const
{
    if (type == Type::VecCond) {
        op->emit(false);
    } else {
        cond->emit();
    }
}

sfpi_inline const VecBool VecBool::operator!() const
{
    VecBool result(*this);
    result.negate = true;
    return result;
}

// Recursively process the conditional tree
// gcc inline bails out due to complexity and always_inline doesn't allow
// recursive inlining.  Instead, manually expand the code with "recursive"
// macro usage up to reasonable compile time limits.  Eventually this has to
// be pushed down into the compiler
// Use De Morgan's laws to convert || to !&& pass the ! down as we go
// Each node may be negated from above, if the current node is an OR, then it
// always negates itself and upates the propagated negate by flipping it
#define __sfpi_cond_emit_loop(leftside, rightside)                                  \
{                                                                       \
    const VecBool* local_node = node;                                  \
    bool negate_node = node->negate;                                    \
    bool negate_child = negate;                                         \
                                                                        \
    VecBool::VecBoolType node_type = negate ? node->get_neg_type() : node->get_type(); \
    if (node_type == VecBool::VecBoolType::Or) {                      \
        negate_node = !negate_node;                                     \
        negate_child = !negate;                                         \
    }                                                                   \
                                                                        \
    if (node->op_a.get_type() == CondOperand::Type::VecBool) {         \
        node = &node->op_a.get_cond();                                  \
        leftside;                                                       \
        node = local_node;                                              \
    } else {                                                            \
        node->op_a.get_op().emit(negate_child);                         \
    }                                                                   \
                                                                        \
    if (node->op_b.get_type() == CondOperand::Type::VecBool) {         \
        node = &node->op_b.get_cond();                                  \
        rightside;                                                      \
        node = local_node;                                              \
    } else {                                                            \
        node->op_b.get_op().emit(negate_child);                         \
    }                                                                   \
                                                                        \
    if (negate_node) {                                                  \
        __builtin_rvtt_sfpcompc();                                      \
    }                                                                   \
}

#define __sfpi_cond_emit_loop_mid(leftside, rightside)                              \
    bool negate = negate_child;                                         \
    __sfpi_cond_emit_loop(leftside, rightside)

#define __sfpi_cond_emit_error __builtin_rvtt_sfpillegal();
sfpi_inline void VecBool::emit(bool negate) const
{
    const VecBool* node = this;

    __sfpi_cond_emit_loop(
        __sfpi_cond_emit_loop_mid(__sfpi_cond_emit_loop(__sfpi_cond_emit_error, __sfpi_cond_emit_error),
                                  __sfpi_cond_emit_loop(__sfpi_cond_emit_error, __sfpi_cond_emit_error)),
        __sfpi_cond_emit_loop_mid(__sfpi_cond_emit_loop(__sfpi_cond_emit_error, __sfpi_cond_emit_error),
                                  __sfpi_cond_emit_loop(__sfpi_cond_emit_error, __sfpi_cond_emit_error)));
}

sfpi_inline VecCond::VecCond(CondOpType t, const VecShort a, int32_t i, uint32_t m, uint32_t nm) :
    type(t), op_a(a), op_b(), imm(i), mod1(m), neg_mod1(nm)
{
}

sfpi_inline const VecCond VecCond::operator!() const
{
    VecCond result(*this);

    result.mod1 = neg_mod1;
    result.neg_mod1 = mod1;

    return result;
}

sfpi_inline void VecCond::emit(bool negate) const
{
    uint32_t use_mod1 = negate ? neg_mod1 : mod1;

    switch (type) {
    case VecCond::CondOpType::CompareFloat:
        __builtin_rvtt_sfpscmp_ex(op_a.get_vec().get(), op_b.get_scalarfp().get(),
                                  use_mod1 | (op_b.get_scalarfp().get_format() ==
                                              SFPLOADI_MOD0_FLOATA ? SFPSCMP_EX_MOD1_FMT_A : 0));
        break;

    case VecCond::CondOpType::CompareVecHalf:
        __builtin_rvtt_sfpvcmp_ex(op_a.get_vec().get(), op_b.get_vec().get(), use_mod1);
        break;

    case VecCond::CondOpType::CompareShort:
        __builtin_rvtt_sfpiadd_i_ex(op_a.get_vec().get(), imm, use_mod1 | SFPIADD_EX_MOD1_IS_SUB);
        break;

    case VecCond::CondOpType::CompareVecShort:
        // Remember: dst gets negated
        __builtin_rvtt_sfpiadd_v_ex(op_b.get_vec().get(), op_a.get_vec().get(),
                                    use_mod1 | SFPIADD_EX_MOD1_IS_SUB);
        break;

    case VecCond::CondOpType::ExExp:
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfpexexp(op_a.get_vec().get(), use_mod1);
        break;

    case VecCond::CondOpType::Lz:
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfplz(op_a.get_vec().get(), use_mod1);
        break;

    case VecCond::CondOpType::IAddI:
        // This is legacy code for add_cc implementation
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfpiadd_i(imm, op_a.get_vec().get(), use_mod1);
        break;

    case VecCond::CondOpType::IAddV:
        // This is legacy code for add_cc implementation
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfpiadd_v(op_b.get_vec_ptr()->get(),
                                                             op_a.get_vec().get(),
                                                             use_mod1);
        break;
    }
}

sfpi_inline CondComp::CondCompOpType CondComp::not_cond(const CondCompOpType t) const
{
    switch (t) {
    case CompLT0:
        return CompGTE0;
    case CompNE0:
        return CompEQ0;
    case CompGTE0:
        return CompLT0;
    case CompEQ0:
        return CompNE0;
    default:
        // Should never get here
        return CompNE0;
    }
}

sfpi_inline ExExpCC CondOpExExp::not_cond(const ExExpCC t) const
{
    return (t == ExExpCCLT0) ? ExExpCCGTE0 : ExExpCCLT0;
}

sfpi_inline LzCC CondOpLz::not_cond(const LzCC t) const
{
    return (t == LzCCNE0) ? LzCCEQ0 : LzCCNE0;
}

sfpi_inline IAddCC CondOpIAddI::not_cond(const IAddCC t) const
{
    return (t == IAddCCLT0) ? IAddCCGTE0 : IAddCCLT0;
}

sfpi_inline IAddCC CondOpIAddV::not_cond(const IAddCC t) const
{
    return (t == IAddCCLT0) ? IAddCCGTE0 : IAddCCLT0;
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline CCCtrl::CCCtrl() : push_count(0)
{
    push();
}

sfpi_inline void CCCtrl::cc_if(const VecBool& op) const
{
    op.emit();
}

sfpi_inline void CCCtrl::cc_if(const VecCond& op) const
{
    op.emit(false);
}

sfpi_inline void CCCtrl::cc_elseif(const VecBool& op)
{
    cc_if(op);
}

sfpi_inline void CCCtrl::cc_elseif(const VecCond& op)
{
    cc_if(op);
}

sfpi_inline void CCCtrl::cc_else() const
{
    __builtin_rvtt_sfpcompc();
}

sfpi_inline CCCtrl::~CCCtrl()
{
    while (push_count != 0) {
        pop();
    }
}

sfpi_inline void CCCtrl::push()
{
    push_count++;
    __builtin_rvtt_sfppushc();
}

sfpi_inline void CCCtrl::pop()
{
    push_count--;
    __builtin_rvtt_sfppopc();
}

sfpi_inline void CCCtrl::enable_cc()
{
    __builtin_rvtt_sfpencc(SFPENCC_IMM12_BOTH, SFPENCC_MOD1_EI_RI);
}

//////////////////////////////////////////////////////////////////////////////
// Functional math library
//////////////////////////////////////////////////////////////////////////////
 sfpi_inline VecHalf lut(const VecHalf v, const VecUShort l0, const VecUShort l1, const VecUShort l2, const int offset = 0)
{
    unsigned int bias_mask = (offset == 1) ? SFPLUT_MOD0_BIAS_POS :
        ((offset == -1) ? SFPLUT_MOD0_BIAS_NEG : SFPLUT_MOD0_BIAS_NONE);
    return __builtin_rvtt_sfplut(l0.get(), l1.get(), l2.get(), v.get(), SFPLUT_MOD0_SGN_RETAIN | bias_mask);
}

sfpi_inline VecHalf lut_sign(const VecHalf v, const VecUShort l0, const VecUShort l1, const VecUShort l2, const int offset = 0)
{
    unsigned int bias_mask = (offset == 1) ? SFPLUT_MOD0_BIAS_POS :
        ((offset == -1) ? SFPLUT_MOD0_BIAS_NEG : SFPLUT_MOD0_BIAS_NONE);
    return __builtin_rvtt_sfplut(l0.get(), l1.get(), l2.get(), v.get(), SFPLUT_MOD0_SGN_UPDATE | bias_mask);
}

sfpi_inline VecShort exexp(const VecHalf v)
{
    return __builtin_rvtt_sfpexexp(v.get(), SFPEXEXP_MOD1_DEBIAS);
}

sfpi_inline VecShort exexp_nodebias(const VecHalf v)
{
    return __builtin_rvtt_sfpexexp(v.get(), SFPEXEXP_MOD1_NODEBIAS);
}

sfpi_inline VecShort exman8(const VecHalf v)
{
    return __builtin_rvtt_sfpexman(v.get(), SFPEXMAN_MOD1_PAD8);
}

sfpi_inline VecShort exman9(const VecHalf v)
{
    return __builtin_rvtt_sfpexman(v.get(), SFPEXMAN_MOD1_PAD9);
}

sfpi_inline VecHalf setexp(const VecHalf v, const uint32_t exp)
{
    return __builtin_rvtt_sfpsetexp_i(exp, v.get());
}

sfpi_inline VecHalf setexp(const VecHalf v, const VecShortBase exp)
{
    // Odd: dst is both exponent and result so undergoes a type change
    // If exp is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetexp_v(exp.get(), v.get());
}

sfpi_inline VecHalf setman(const VecHalf v, const uint32_t man)
{
    return __builtin_rvtt_sfpsetman_i(man, v.get());
}

sfpi_inline VecHalf setman(const VecHalf v, const VecShortBase man)
{
    // Grayskull HW bug, is this useful?  Should there be a "Half" form?
    // Odd: dst is both man and result so undergoes a type change
    // If man is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetman_v(man.get(), v.get());
}

sfpi_inline VecHalf addexp(const VecHalf in, const int32_t exp)
{
    return __builtin_rvtt_sfpdivp2(exp, in.get(), SFPSDIVP2_MOD1_ADD);
}

sfpi_inline VecHalf setsgn(const VecHalf v, const int32_t sgn)
{
    return __builtin_rvtt_sfpsetsgn_i(sgn, v.get());
}

sfpi_inline VecHalf setsgn(const VecHalf v, const VecHalf sgn)
{
    // Odd: dst is both sgn and result so undergoes a type change
    // If sgn is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetsgn_v(sgn.get(), v.get());
}

sfpi_inline VecHalf setsgn(const VecHalf v, const VecShort sgn)
{
    // Odd: dst is both sgn and result so undergoes a type change
    // If sgn is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetsgn_v(sgn.get(), v.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<Vec, vType>::value>* = nullptr>
sfpi_inline VecShort lz(const vType v)
{
    return VecShort(__builtin_rvtt_sfplz(v.get(), SFPLZ_MOD1_CC_NONE));
}

sfpi_inline VecHalf abs(const VecHalf v)
{
    return __builtin_rvtt_sfpabs(v.get(), SFPABS_MOD1_FLOAT);
}

sfpi_inline VecShort abs(const VecShort v)
{
    return __builtin_rvtt_sfpabs(v.get(), SFPABS_MOD1_INT);
}

sfpi_inline VecUShort shft(const VecUShort v, const VecShort amt)
{
    return __builtin_rvtt_sfpshft_v(v.get(), amt.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<Vec, vType>::value>* = nullptr>
sfpi_inline vType reinterpret(const Vec v)
{
    return vType(v.get());
}

} // namespace sfpi


#define p_if(x)             \
{                           \
    CCCtrl __cc;            \
    __cc.cc_if(x);

#define p_elseif(x)         \
    __cc.cc_else();         \
    __cc.push();            \
    __cc.cc_elseif(x);

#define p_else              \
    __cc.cc_else();

#define p_endif             \
}

#define p_block             \
{                           \
    CCCtrl __cc;

#define p_and(x)            \
    __cc.cc_if(x)

#define p_endblock          \
}
