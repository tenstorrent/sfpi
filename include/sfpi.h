///////////////////////////////////////////////////////////////////////////////
// sfpi.h: SFPu Interface
//   This file provides a C++ wrapper around GCC style __builtin functions
//   which map to SFPU instructions.
//
// Note: Op refers to "Operation" while vOperand is spelled out.  Usually.
//
// Local Register (LREG) Operations:
//   class vUInt
//   class vInt
//   class vFloat
//
//   These classes are the workhorse classes of sfpi.  Operators are
//   overloaded to provide functionality such as:
//     vFloat a, b, c, d;
//     vFloat d = a * b + c;
//
//   The classes also support conditional operators for use in conjunction
//   with the vCCCtrl class to enable predicated execution, for example:
//     vFloat a;
//     v_if (a < 5.0F)
//   will load an immediate value of 5.0F into an LReg, perform the subtract
//   and test the result to set the CC.
//
//   Additional operations such as load immediate, extract/set exp, etc are
//   supported through class methods.
//
// Destination Register:
//   class vDReg
//   constexpr RegFile<vDReg, 64> dst_reg;
//
//   The Destination Register is modeled by a global variable which is
//   essentially an array of class vDReg.  vDRegs provide much the same
//   functionality as LRegs, eg, the following is legal:
//       dst_reg[0] = dst_reg[1] * dst_reg[2] + dst_reg[3];
//   The above is expanded out to load local registers, perform the operation
//   and store back into the destination register.  Any missing functionality
//   (eg, there is no load immediate) accessed through LRegs.
//
// Constant Local Registers:
//   class vConst
//   constexpr vConst vConst0p6928(5);
//
//   The constant value local registers are used in expressions by referencing
//   one of the names above and using them in mathematical operations such as:
//       a = b * c + vConst0p6928;
//
// Predicated Execution:
//   class vCCCtrl
//   macros v_if(), v_elseif(), v_else, v_endif
//
//   The class vCCCtrl is used in conjunction with the LReg based classes to
//   enable predicated execution.  By convention the test infastructure indents
//   the code as if executing if/then/else in C++, for example:
//     vCCCtrl cc;
//     vFloat v = dst_reg[0];
//     cc.cc_if(v < 5.0F); {
//         // if side
//     } cc.cc_else(); {
//         // else side
//     }
//     cc.cc_endif();
//   The above chatter is reduced w/ use of macros listed above.  The macros
//   also balance {} to auto-destruct the vCCCtrl when appropriate.
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
//    example, assigning x, then executing a v_if and re-assigning x within
//    the predicate keeps x "alive" rather than letting it go dead prior to
//    the second assignment.  The solution for this is a hybrid
//    wrapper/compiler solution; a document exists which describes this in
//    detail.  The main piece for the wrapper is that assignments generate an
//    sfpassign_lv instruction which propogates the value from the previous
//    assignment, thought this can only be done if the variable was previously
//    written (initialized) and is only necessary if the instruction doesn't
//    take the destination as an input (otherwise an unnecessary move may be
//    generated).
//
//  Ugliness:
//    The wrapper should get smaller over time as functionality is moved into
//    the compiler back end (optimizer) and front end (parser, eg, v_endif).
//    Currently the biggest uglinesses are:
//      v_endif
//      assignments return void so cannot be used in, eg conditionals
//        since the compiler doesn't understand predication, using assignments
//        in condtionals can result in evaluation of the assignments out of
//        order w/ regard to the predication CC bits
//      related to the above, add_cc, exexp_cc and lz_cc are exposed to handle
//        instructions within conditionals
//      && and || can only go 3 deep in one conditional
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
class vConst;
class vBase;
class vFloat;
class vIntBase;
class vInt;
class vUInt;
class vCond;
class vBool;
class vCondComp;
class vCondOpExExp;
class vCondOpIAddI;
class vCondOpIAddV;
class vCondOpLz;
class vCCCtrl;
class vCCCtrlBase;

//////////////////////////////////////////////////////////////////////////////
template<class Type, int N>
class RegFile {

public:
    constexpr Type operator[](const int x) const;
};

//////////////////////////////////////////////////////////////////////////////
class vRegBase {
protected:
    int reg;

public:
    constexpr explicit vRegBase(int r) : reg(r) {}
    constexpr int get() const { return reg; }
};

//////////////////////////////////////////////////////////////////////////////
class vRegBaseInitializer {
    int n;

 public:
    constexpr vRegBaseInitializer(int in) : n(in) {}
    constexpr int get() const { return n; }
};

//////////////////////////////////////////////////////////////////////////////
class vDReg : public vRegBase {
private:
    template<class Type, int N> friend class RegFile;
    constexpr explicit vDReg(const vRegBaseInitializer i) : vRegBase(i.get() * SFP_DESTREG_STRIDE) {}

public:
    // Assign register to register
    template <typename vecType, typename std::enable_if_t<std::is_base_of<vBase, vecType>::value>* = nullptr>
    sfpi_inline void operator=(const vecType vec) const;
    sfpi_inline void operator=(const vDReg dreg) const;
    sfpi_inline void operator=(const vConst creg) const;
    sfpi_inline void operator=(const s2vFloat16 f) const;

    // Construct operator classes from operations
    sfpi_inline vFloat operator+(const vFloat b) const;
    sfpi_inline vFloat operator-(const vFloat b) const;
    sfpi_inline vFloat operator-() const;
    sfpi_inline vFloat operator*(const vFloat b) const;

    // Conditionals
    sfpi_inline vCondComp operator==(const float x) const;
    sfpi_inline vCondComp operator!=(const float x) const;
    sfpi_inline vCondComp operator<(const float x) const;
    sfpi_inline vCondComp operator<=(const float x) const;
    sfpi_inline vCondComp operator>(const float x) const;
    sfpi_inline vCondComp operator>=(const float x) const;

    sfpi_inline vCondComp operator==(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator!=(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator<(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator<=(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator>(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator>=(const s2vFloat16 x) const;

    sfpi_inline vCondComp operator==(const vFloat x) const;
    sfpi_inline vCondComp operator!=(const vFloat x) const;
    sfpi_inline vCondComp operator<(const vFloat x) const;
    sfpi_inline vCondComp operator<=(const vFloat x) const;
    sfpi_inline vCondComp operator>(const vFloat x) const;
    sfpi_inline vCondComp operator>=(const vFloat x) const;
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
    friend vBase;

private:
    LRegAssignerInternal lregs[4];

    sfpi_inline void assign(__rvtt_vec_t& in, LRegAssignerInternal& lr);

public:
    sfpi_inline LRegAssigner() : lregs{LRegs::LReg0, LRegs::LReg1, LRegs::LReg2, LRegs::LReg3} {}
    sfpi_inline ~LRegAssigner();
    sfpi_inline LRegAssignerInternal& assign(LRegs lr);
};

//////////////////////////////////////////////////////////////////////////////
class vConst : public vRegBase {
public:
    constexpr explicit vConst(int r) : vRegBase(r) {}

    // Construct operator classes from operations
    sfpi_inline vFloat operator+(const vFloat b) const;
    sfpi_inline vFloat operator-(const vFloat b) const;
    sfpi_inline vFloat operator*(const vFloat b) const;

    sfpi_inline vCondComp operator==(const vFloat x) const;
    sfpi_inline vCondComp operator!=(const vFloat x) const;
    sfpi_inline vCondComp operator<(const vFloat x) const;
    sfpi_inline vCondComp operator<=(const vFloat x) const;
    sfpi_inline vCondComp operator>(const vFloat x) const;
    sfpi_inline vCondComp operator>=(const vFloat x) const;

    // Shifts
    sfpi_inline vIntBase operator<<(uint32_t amt) const;
    sfpi_inline vUInt operator>>(uint32_t amt) const;
};

//////////////////////////////////////////////////////////////////////////////
class vBase {
protected:
    bool initialized;
    __rvtt_vec_t v;

    sfpi_inline void assign(const __rvtt_vec_t t);

public:
    sfpi_inline vBase() : initialized(false) {}

    sfpi_inline __rvtt_vec_t get() const { return v; }
    sfpi_inline __rvtt_vec_t& get() { return v; }

    // Associate variable w/ a value pre-loaded into a particular lreg
    sfpi_inline void operator=(LRegAssignerInternal& lr);
};

//////////////////////////////////////////////////////////////////////////////
class vFloat : public vBase {
private:
    sfpi_inline void loadf(const float val);
    sfpi_inline void loadf16(const s2vFloat16 val);

public:
    vFloat() = default;

    sfpi_inline vFloat(const vDReg dreg);
    sfpi_inline vFloat(const vConst creg);
    sfpi_inline vFloat(const s2vFloat16 f) { loadf16(f); }
    sfpi_inline vFloat(const float f) { loadf(f); }
    sfpi_inline vFloat(const __rvtt_vec_t& t) { assign(t); }

    // Assignment
    sfpi_inline void operator=(const vFloat in) { assign(in.v); }
    sfpi_inline void operator=(LRegAssignerInternal& lr) { vBase::operator=(lr); }

    // Construct operator from operations
    sfpi_inline vFloat operator+(const vFloat b) const;
    sfpi_inline void operator+=(const vFloat);
    sfpi_inline vFloat operator-(const vFloat b) const;
    sfpi_inline vFloat operator-(const float b) const;
    sfpi_inline vFloat operator-(const s2vFloat16 b) const;
    sfpi_inline void operator-=(const vFloat);
    sfpi_inline vFloat operator-() const;
    sfpi_inline vFloat operator*(const vFloat b) const;
    sfpi_inline void operator*=(const vFloat);

    // Conditionals
    sfpi_inline vCondComp operator==(const float x) const;
    sfpi_inline vCondComp operator==(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator==(const vFloat x) const;
    sfpi_inline vCondComp operator!=(const float x) const;
    sfpi_inline vCondComp operator!=(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator!=(const vFloat x) const;
    sfpi_inline vCondComp operator<(const float x) const;
    sfpi_inline vCondComp operator<(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator<(const vFloat x) const;
    sfpi_inline vCondComp operator<=(const float x) const;
    sfpi_inline vCondComp operator<=(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator<=(const vFloat x) const;
    sfpi_inline vCondComp operator>(const float x) const;
    sfpi_inline vCondComp operator>(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator>(const vFloat x) const;
    sfpi_inline vCondComp operator>=(const float x) const;
    sfpi_inline vCondComp operator>=(const s2vFloat16 x) const;
    sfpi_inline vCondComp operator>=(const vFloat x) const;
};

//////////////////////////////////////////////////////////////////////////////
class vIntBase : public vBase {
 protected:
    sfpi_inline void loadsi(int32_t val);
    sfpi_inline void loadui(uint32_t val);

 public:
    vIntBase() = default;
    sfpi_inline vIntBase(const __rvtt_vec_t& in) { assign(in); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<vBase, vType>::value>* = nullptr>
    sfpi_inline explicit operator vType() const { return vType(v); }

    // Bit Operations
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator&(const vType b) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator&=(const vType b);
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator|(const vType b) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator|=(const vType b);
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator^(const vType b) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator^=(const vType b);
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator~() const;

    // Arith
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType add(int32_t val, unsigned int mod) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator+(const vIntBase val) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator+(const vConst val) const;

    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType sub(int32_t val, unsigned int mod) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator-(const vIntBase val) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator-(const vConst val) const;

    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void add_eq(int32_t val, unsigned int mod);
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator+=(const vIntBase val);
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator+=(const vConst val);

    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void sub_eq(int32_t val, unsigned int mod);
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator-=(const vIntBase val);
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator-=(const vConst val);

    // Shifts
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator<<(uint32_t amt) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator<<=(uint32_t amt);

    // Misc
    sfpi_inline const vCondOpLz lz_cc(const vBase src, LzCC cc);
};

class vInt : public vIntBase {
    friend class vUInt;

public:
    vInt() = default;
    sfpi_inline vInt(const __rvtt_vec_t& in) { assign(in); }
    sfpi_inline vInt(const vConst creg) { v = __builtin_rvtt_sfpassignlr(creg.get()); initialized = true; }
    sfpi_inline vInt(const vIntBase in) { assign(in.get()); };
    sfpi_inline vInt(short val) { loadsi(val); }
    sfpi_inline vInt(int val) { loadsi(val); }
#ifndef __clang__
    sfpi_inline vInt(int32_t val) { loadsi(val); }
#endif
    sfpi_inline vInt(unsigned short val) { loadui(val); }
    sfpi_inline vInt(unsigned int val) { loadui(val); }
#ifndef __clang__
    sfpi_inline vInt(uint32_t val) { loadui(val); }
#endif

    // Assignment
    sfpi_inline void operator=(const vInt in) { assign(in.v); }
    sfpi_inline void operator=(LRegAssignerInternal& lr) { vBase::operator=(lr); }

    // Operations
    sfpi_inline void operator&=(const vInt b) { return this->vIntBase::operator&=(b); }
    sfpi_inline void operator|=(const vInt b) { return this->vIntBase::operator|=(b); }
    sfpi_inline vInt operator~() const { return this->vIntBase::operator~<vInt>(); }

    sfpi_inline vInt operator+(int32_t val) const { return this->vIntBase::add<vInt>(val, SFPIADD_I_EX_MOD1_SIGNED); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vInt operator+(const vType val) const { return this->vIntBase::operator+<vInt>(val); }
    sfpi_inline vInt operator+(const vConst val) const { return this->vIntBase::operator+<vInt>(val); }

    sfpi_inline vInt operator-(int32_t val) const { return this->vIntBase::sub<vInt>(val, SFPIADD_I_EX_MOD1_SIGNED); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vInt operator-(const vType val) const { return this->vIntBase::operator-<vInt>(val); }
    sfpi_inline vInt operator-(const vConst val) const { return this->vIntBase::operator-<vInt>(val); }

    sfpi_inline void operator+=(int32_t val) { this->vIntBase::add_eq<vInt>(val, SFPIADD_I_EX_MOD1_SIGNED); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator+=(const vType val) { this->vIntBase::operator+=<vInt>(val); }
    sfpi_inline void operator+=(const vConst val) { this->vIntBase::operator-=<vInt>(val); }

    sfpi_inline void operator-=(int32_t val) { this->vIntBase::sub_eq<vInt>(val, SFPIADD_I_EX_MOD1_SIGNED); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator-=(const vType val) { this->vIntBase::operator-=<vInt>(val); }
    sfpi_inline void operator-=(const vConst val) { this->vIntBase::operator-=<vInt>(val); }

    sfpi_inline vInt operator<<(uint32_t amt) const { return this->vIntBase::operator<<<vInt>(amt); }
    sfpi_inline void operator<<=(uint32_t amt) { return this->vIntBase::operator<<=<vInt>(amt); }

    // Conditionals
    sfpi_inline const vCondComp operator==(int32_t val) const;
    sfpi_inline const vCondComp operator!=(int32_t val) const;
    sfpi_inline const vCondComp operator<(int32_t val) const;
    sfpi_inline const vCondComp operator<=(int32_t val) const;
    sfpi_inline const vCondComp operator>(int32_t val) const;
    sfpi_inline const vCondComp operator>=(int32_t val) const;

    sfpi_inline const vCondComp operator==(const vIntBase src) const;
    sfpi_inline const vCondComp operator!=(const vIntBase src) const;
    sfpi_inline const vCondComp operator<(const vIntBase src) const;
    sfpi_inline const vCondComp operator<=(const vIntBase src) const;
    sfpi_inline const vCondComp operator>(const vIntBase src) const;
    sfpi_inline const vCondComp operator>=(const vIntBase src) const;

    sfpi_inline const vCondOpExExp exexp_cc(vFloat src, const ExExpCC cc);
    sfpi_inline const vCondOpExExp exexp_nodebias_cc(vFloat src, const ExExpCC cc);
    sfpi_inline const vCondOpLz lz_cc(const vBase src, LzCC cc);
    sfpi_inline const vCondOpIAddI add_cc(const vInt src, int32_t val, IAddCC cc);
    sfpi_inline const vCondOpIAddV add_cc(const vInt src, IAddCC cc);
};

//////////////////////////////////////////////////////////////////////////////
class vUInt : public vIntBase {
    friend class vInt;

private:

public:
    vUInt() = default;
    sfpi_inline vUInt(const __rvtt_vec_t& in) { assign(in); }
    sfpi_inline vUInt(const vConst creg) { v = __builtin_rvtt_sfpassignlr(creg.get()); initialized = true; }
    sfpi_inline vUInt(const vIntBase in) { assign(in.get()); }
    sfpi_inline vUInt(short val) { loadsi(val); }
    sfpi_inline vUInt(int val) { loadsi(val); }
#ifndef __clang__
    sfpi_inline vUInt(int32_t val) { loadsi(val); }
#endif
    sfpi_inline vUInt(unsigned short val) { loadui(val); }
    sfpi_inline vUInt(unsigned int val) { loadui(val); }
#ifndef __clang__
    sfpi_inline vUInt(uint32_t val) { loadui(val); }
#endif

    // Assignment
    sfpi_inline void operator=(const vUInt in ) { assign(in.v); }
    sfpi_inline void operator=(LRegAssignerInternal& lr) { vBase::operator=(lr); }

    // Operations
    sfpi_inline void operator&=(const vUInt b) { return this->vIntBase::operator&=(b); }
    sfpi_inline void operator|=(const vUInt b) { return this->vIntBase::operator|=(b); }
    sfpi_inline vUInt operator~() const { return this->vIntBase::operator~<vUInt>(); }

    sfpi_inline vUInt operator+(int32_t val) const { return this->vIntBase::add<vUInt>(val, 0); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vUInt operator+(const vType val) const { return this->vIntBase::operator+<vUInt>(val); }
    sfpi_inline vUInt operator+(const vConst val) const { return this->vIntBase::operator+<vUInt>(val); }

    sfpi_inline vUInt operator-(int32_t val) const { return this->vIntBase::sub<vUInt>(val, 0); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline vUInt operator-(const vType val) const { return this->vIntBase::operator-<vUInt>(val); }
    sfpi_inline vUInt operator-(const vConst val) const { return this->vIntBase::operator+<vUInt>(val); }

    sfpi_inline void operator+=(int32_t val) { this->vIntBase::add_eq<vUInt>(val, 0); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator+=(const vType val) { this->vIntBase::operator+=<vUInt>(val); }
    sfpi_inline void operator+=(const vConst val) { this->vIntBase::operator+=<vUInt>(val); }

    sfpi_inline void operator-=(int32_t val) { this->vIntBase::sub_eq<vUInt>(val, 0); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator-=(const vType val) { this->vIntBase::operator-=<vUInt>(val); }
    sfpi_inline void operator-=(const vConst val) { this->vIntBase::operator-=<vUInt>(val); }

    sfpi_inline vUInt operator<<(uint32_t amt) const { return this->vIntBase::operator<<<vUInt>(amt); }
    sfpi_inline vUInt operator>>(uint32_t amt) const;
    sfpi_inline void operator<<=(uint32_t amt) { this->vIntBase::operator<<=<vUInt>(amt); }
    sfpi_inline void operator>>=(uint32_t amt);

    // Conditionals
    sfpi_inline const vCondComp operator==(int32_t val) const;
    sfpi_inline const vCondComp operator!=(int32_t val) const;
    sfpi_inline const vCondComp operator<(int32_t val) const;
    sfpi_inline const vCondComp operator<=(int32_t val) const;
    sfpi_inline const vCondComp operator>(int32_t val) const;
    sfpi_inline const vCondComp operator>=(int32_t val) const;

    sfpi_inline const vCondComp operator==(const vIntBase src) const;
    sfpi_inline const vCondComp operator!=(const vIntBase src) const;
    sfpi_inline const vCondComp operator<(const vIntBase src) const;
    sfpi_inline const vCondComp operator<=(const vIntBase src) const;
    sfpi_inline const vCondComp operator>(const vIntBase src) const;
    sfpi_inline const vCondComp operator>=(const vIntBase src) const;

    sfpi_inline const vCondOpIAddI add_cc(const vUInt src, int32_t val, IAddCC cc);
    sfpi_inline const vCondOpIAddV add_cc(const vUInt src, IAddCC cc);
};

//////////////////////////////////////////////////////////////////////////////
class vOperand {
public:
    enum class Type {
        Null,
        Vec,
        VecPtr,
        Float16,
    };

private:
    const Type kind;

    union {
        // Some __builtins require the dst LREG as an vOperand, can't be const
        // Ripe for const abuse
        const void* const null_ptr;
        const vBase vec;
        vBase* const vec_ptr;
        const uint32_t uint_val;
        const s2vFloat16 s2vfloat16_val;
    };

    const bool negate;

public:
    sfpi_inline vOperand() : kind(Type::Null), null_ptr(nullptr), negate(false) {}
    sfpi_inline vOperand(const vBase v, bool neg = false) : kind(Type::Vec), vec(v), negate(neg) {}
    sfpi_inline vOperand(vBase* const v, bool neg = false) : kind(Type::VecPtr), vec_ptr(v), negate(neg) {}
    sfpi_inline vOperand(const s2vFloat16 v) : kind(Type::Float16), s2vfloat16_val(v), negate(false) {}

    sfpi_inline Type get_type() const { return kind; }
    sfpi_inline bool is_neg() const { return negate; }

    sfpi_inline const vBase get_vec() const { return vec; }
    sfpi_inline vBase* const get_vec_ptr() const { return vec_ptr; }
    sfpi_inline const s2vFloat16 get_scalarfp() const { return s2vfloat16_val; }
};

//////////////////////////////////////////////////////////////////////////////
// Handle conditionals.  Leaves are vCond, vBool.
class vCondOperand {
public:
    enum class Type {
        vCond,
        vBool,
    };

private:
    const Type type;
    union {
        const vCond* const op;
        const vBool* const cond;
    };

public:
    sfpi_inline vCondOperand(const vCond& a) : type(Type::vCond), op(&a) {}
    sfpi_inline vCondOperand(const vBool& a) : type(Type::vBool), cond(&a) {}

    sfpi_inline Type get_type() const { return type; }
    sfpi_inline const vBool& get_cond() const { return *cond; }
    sfpi_inline const vCond& get_op() const { return *op; }

    sfpi_inline void emit(vCCCtrl *cc) const;
};

//////////////////////////////////////////////////////////////////////////////
// Build a tree of conditionals as the compiler parses them
// Traverse the tree at emit time to generate the code
class vBool {
public:
    enum class vBoolType {
        Or,
        And,
    };

private:
    const vBoolType type;
    vCondOperand op_a;
    vCondOperand op_b;
    bool negate;

public:
    sfpi_inline vBool(vBoolType t, const vCond& a, const vCond& b) : type(t), op_a(a), op_b(b), negate(false) {}
    sfpi_inline vBool(vBoolType t, const vCond& a, const vBool& b) : type(t), op_a(a), op_b(b), negate(false) {}
    sfpi_inline vBool(vBoolType t, const vBool& a, const vCond& b) : type(t), op_a(a), op_b(b), negate(false) {}
    sfpi_inline vBool(vBoolType t, const vBool& a, const vBool& b) : type(t), op_a(a), op_b(b), negate(false) {}

    sfpi_inline const vBool operator!() const;

    sfpi_inline const vBool operator&&(const vCond& b) const { return vBool(vBoolType::And, *this, b); }
    sfpi_inline const vBool operator||(const vCond& b) const { return vBool(vBoolType::Or, *this, b); }
    sfpi_inline const vBool operator&&(const vBool& b) const { return vBool(vBoolType::And, *this, b); }
    sfpi_inline const vBool operator||(const vBool& b) const { return vBool(vBoolType::Or, *this, b); }

    sfpi_inline vBoolType get_type() const { return type; }
    sfpi_inline vBoolType get_neg_type() const { return (type == vBoolType::Or) ? vBoolType::And : vBoolType::Or; }
    sfpi_inline const vCondOperand& get_op_a() const { return op_a; }
    sfpi_inline const vCondOperand& get_op_b() const { return op_b; }

    sfpi_inline void emit(vCCCtrl* cc, bool negate = false) const;
};

//////////////////////////////////////////////////////////////////////////////
class vCond {
protected:
    enum class vCondOpType {
        CompareFloat,
        ComparevFloat,
        CompareShort,
        ComparevInt,
        ExExp,
        Lz,
        IAddI,
        IAddV
    };

private:
    const vCondOpType type;
    const vOperand op_a;
    vOperand op_b;
    const int32_t imm;
    uint32_t mod1;
    uint32_t neg_mod1;
    bool comp;
    bool neg_comp;

public:
    sfpi_inline vCond(vCondOpType t) : type(t), op_a(), op_b(), imm(0), mod1(0), neg_mod1(0), comp(false), neg_comp(false) {}

    template <class typeA, class typeB>
    sfpi_inline vCond(vCondOpType t, const typeA a, const typeB b, int32_t i, uint32_t m, uint32_t nm, bool c = false, bool nc = false) : type(t), op_a(a), op_b(b), imm(i), mod1(m), neg_mod1(nm), comp(c), neg_comp(nc) {}

    template <class typeA>
    sfpi_inline vCond(vCondOpType t, const typeA a, vBase* const b, int32_t i, uint32_t m, uint32_t nm, bool c= false, bool nc = false) : type(t), op_a(a), op_b(b), imm(i), mod1(m), neg_mod1(nm), comp(c), neg_comp(nc) {}

    template <class typeA>
    sfpi_inline vCond(vCondOpType t, const typeA a, int32_t i, uint32_t m, uint32_t nm, bool c = false, bool nc = false) : type(t), op_a(a), op_b(), imm(i), mod1(m), neg_mod1(nm), comp(c), neg_comp(nc) {}

    template <class typeA>
    sfpi_inline vCond(vCondOpType t, const typeA a, vIntBase b, uint32_t m, uint32_t nm, bool c = false, bool nc = false) : type(t), op_a(a), op_b(b), imm(0), mod1(m), neg_mod1(nm), comp(c), neg_comp(nc) {}

    template <class typeA>
    sfpi_inline vCond(vCondOpType t, const typeA a, const s2vFloat16 b, int32_t i, uint32_t m, uint32_t nm, bool c = false, bool nc = false) : type(t), op_a(a), op_b(b), imm(i), mod1(m), neg_mod1(nm), comp(c), neg_comp(nc) {}

    sfpi_inline vCond(vCondOpType t, const vInt a, int32_t i, uint32_t m, uint32_t nm, bool c = false, bool n = false);

    // Negate
    sfpi_inline const vCond operator!() const;

    // Create boolean operations from conditional operations
    sfpi_inline const vBool operator&&(const vCond& b) const { return vBool(vBool::vBoolType::And, *this, b); }
    sfpi_inline const vBool operator||(const vCond& b) const { return vBool(vBool::vBoolType::Or, *this, b); }
    sfpi_inline const vBool operator&&(const vBool& b) const { return vBool(vBool::vBoolType::And, *this, b); }
    sfpi_inline const vBool operator||(const vBool& b) const { return vBool(vBool::vBoolType::Or, *this, b); }

    sfpi_inline void emit(bool negate) const;

    sfpi_inline bool issues_compc(bool negate) const { return negate ? neg_comp : comp; }
};

//////////////////////////////////////////////////////////////////////////////
// Conditional Comparison
class vCondComp : public vCond {
public:
    enum vCondCompOpType {
        CompLT = SFPCMP_EX_MOD1_CC_LT,
        CompNE = SFPCMP_EX_MOD1_CC_NE,
        CompGTE = SFPCMP_EX_MOD1_CC_GTE,
        CompEQ = SFPCMP_EX_MOD1_CC_EQ,
        CompLTE = SFPCMP_EX_MOD1_CC_LTE,
        CompGT = SFPCMP_EX_MOD1_CC_GT,
    };

private:
    sfpi_inline vCondCompOpType not_cond(const vCondCompOpType t) const;

public:
    template <class type>
    sfpi_inline vCondComp(const vCondCompOpType t, const type a, const s2vFloat16 b, bool c = false, bool nc = false) : vCond(vCondOpType::CompareFloat, a, b, 0, t, not_cond(t), c, nc) {}

    sfpi_inline vCondComp(const vCondCompOpType t, const vFloat a, const vFloat b, bool c = false, bool nc = false) : vCond(vCondOpType::ComparevFloat, a, b, 0, t, not_cond(t), c, nc) {}

    template <class type>
    sfpi_inline vCondComp(const vCondCompOpType t, const type a, const int32_t b, const unsigned int mod, bool c = false, bool nc = false) : vCond(vCondOpType::CompareShort, a, b, t | mod, not_cond(t) | mod, c, nc) {}

    template <class type>
        sfpi_inline vCondComp(const vCondCompOpType t, const type a, const vIntBase b, const unsigned int mod, bool c = false, bool nc = false) : vCond(vCondOpType::ComparevInt, a, b, t | mod, not_cond(t) | mod, c, nc) {}
};

//////////////////////////////////////////////////////////////////////////////
// Conditional Operations

class vCondOpExExp : public vCond {
    sfpi_inline ExExpCC not_cond(ExExpCC cc) const;

public:
    sfpi_inline vCondOpExExp(vBase* const d, const vBase s, unsigned short debias, ExExpCC cc) : vCond(vCondOpType::ExExp, s, d, 0, debias | cc, debias | not_cond(cc)) {}
};

class vCondOpLz : public vCond {
    sfpi_inline LzCC not_cond(LzCC cc) const;

public:
    sfpi_inline vCondOpLz(vBase* const d, const vBase s, LzCC cc) : vCond(vCondOpType::Lz, s, d, 0, cc, not_cond(cc)) {}
};

class vCondOpIAddI : public vCond {
    sfpi_inline IAddCC not_cond(IAddCC cc) const;

public:
    sfpi_inline vCondOpIAddI(vBase* const d, const vBase s, IAddCC cc, int32_t imm) : vCond(vCondOpType::IAddI, s, d, imm, cc | SFPIADD_MOD1_ARG_IMM, not_cond(cc) | SFPIADD_MOD1_ARG_IMM) {}
};

class vCondOpIAddV : public vCond {
    sfpi_inline IAddCC not_cond(IAddCC cc) const;

public:
    sfpi_inline vCondOpIAddV(vBase* d, const vBase s, IAddCC cc) : vCond(vCondOpType::IAddV, s, d, 0, cc, not_cond(cc)) {}
};

//////////////////////////////////////////////////////////////////////////////
class vCCCtrl {
protected:
    int push_count;

public:
    sfpi_inline vCCCtrl();
    sfpi_inline ~vCCCtrl();

    sfpi_inline void cc_if(const vBool& op);
    sfpi_inline void cc_if(const vCond& op);
    sfpi_inline void cc_else() const;
    sfpi_inline void cc_elseif(const vBool& cond);
    sfpi_inline void cc_elseif(const vCond& cond);

    sfpi_inline void push();
    sfpi_inline void pop();

    static sfpi_inline void enable_cc();
};

//////////////////////////////////////////////////////////////////////////////
constexpr vConst vConst0(CREG_IDX_0);
constexpr vConst vConst0p6929(CREG_IDX_0P692871094);
constexpr vConst vConstNeg1p0068(CREG_IDX_NEG_1P00683594);
constexpr vConst vConst1p4424(CREG_IDX_1P442382813);
constexpr vConst vConst0p8369(CREG_IDX_0P836914063);
constexpr vConst vConstNeg0p5(CREG_IDX_NEG_0P5);
constexpr vConst vConst1(CREG_IDX_1);
constexpr vConst vConstNeg1(CREG_IDX_NEG_1);
constexpr vConst vConst0p0020(CREG_IDX_0P001953125);
constexpr vConst vConstNeg0p6748(CREG_IDX_NEG_0P67480469);
constexpr vConst vConstNeg0p3447(CREG_IDX_NEG_0P34472656);
constexpr vConst vConstTileId(CREG_IDX_TILEID);

constexpr RegFile<vDReg, 64> dst_reg;

//////////////////////////////////////////////////////////////////////////////
namespace sfpi_int {

sfpi_inline vFloat fp_add(const vFloat a, const vFloat b)
{
    return __builtin_rvtt_sfpadd(a.get(), b.get(), 0);
}

sfpi_inline vFloat fp_sub(const vFloat a, const vFloat b)
{
    __rvtt_vec_t neg1 = __builtin_rvtt_sfpassignlr(vConstNeg1.get());
    return __builtin_rvtt_sfpmad(neg1, b.get(), a.get(), 0);
}

sfpi_inline vFloat fp_mul(const vFloat a, const vFloat b)
{
    return __builtin_rvtt_sfpmul(a.get(), b.get(), 0);
}

}

//////////////////////////////////////////////////////////////////////////////
template<class TYPE, int N>
constexpr TYPE RegFile<TYPE, N>::operator[](const int x) const {
    return TYPE(vRegBaseInitializer(x));
}

sfpi_inline vFloat vDReg::operator+(const vFloat b) const {return sfpi_int::fp_add(vFloat(*this), b); }
sfpi_inline vFloat vDReg::operator-(const vFloat b) const { return sfpi_int::fp_sub(vFloat(*this), b); }
sfpi_inline vFloat vDReg::operator*(const vFloat b) const  { return sfpi_int::fp_mul(vFloat(*this), b); }
sfpi_inline vCondComp vDReg::operator==(const float x) const {return vCondComp(vCondComp::CompEQ, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator!=(const float x) const { return vCondComp(vCondComp::CompNE, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator<(const float x) const { return vCondComp(vCondComp::CompLT, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator<=(const float x) const { return vCondComp(vCondComp::CompLTE, vFloat(*this), s2vFloat16(x), true, false); }
sfpi_inline vCondComp vDReg::operator>(const float x) const { return vCondComp(vCondComp::CompGT, vFloat(*this), s2vFloat16(x), false, true); }
sfpi_inline vCondComp vDReg::operator>=(const float x) const { return vCondComp(vCondComp::CompGTE, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator==(const s2vFloat16 x) const {return vCondComp(vCondComp::CompEQ, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator!=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompNE, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator<(const s2vFloat16 x) const { return vCondComp(vCondComp::CompLT, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator<=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompLTE, vFloat(*this), x, true, false); }
sfpi_inline vCondComp vDReg::operator>(const s2vFloat16 x) const { return vCondComp(vCondComp::CompGT, vFloat(*this), x, false, true); }
sfpi_inline vCondComp vDReg::operator>=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompGTE, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator==(const vFloat x) const {return vCondComp(vCondComp::CompEQ, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator!=(const vFloat x) const { return vCondComp(vCondComp::CompNE, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator<(const vFloat x) const { return vCondComp(vCondComp::CompLT, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator<=(const vFloat x) const { return vCondComp(vCondComp::CompLTE, vFloat(*this), x, true, false); }
sfpi_inline vCondComp vDReg::operator>(const vFloat x) const { return vCondComp(vCondComp::CompGT, vFloat(*this), x, false, true); }
sfpi_inline vCondComp vDReg::operator>=(const vFloat x) const { return vCondComp(vCondComp::CompGTE, vFloat(*this), x); }

template <>
sfpi_inline void vDReg::operator=(const vFloat vec) const
{
    // For  half, rebias
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

template <typename vecType, typename std::enable_if_t<std::is_base_of<vBase, vecType>::value>*>
sfpi_inline void vDReg::operator=(const vecType vec) const
{
    // For short/ushort, do not rebias
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_INT, reg);
}

sfpi_inline void vDReg::operator=(const vDReg dreg) const
{
    vFloat tmp = dreg;
    __builtin_rvtt_sfpstore(tmp.get(), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

sfpi_inline void vDReg::operator=(const vConst creg) const
{
    __rvtt_vec_t lr = __builtin_rvtt_sfpassignlr(creg.get());
    __builtin_rvtt_sfpstore(lr, SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

sfpi_inline void vDReg::operator=(s2vFloat16 f) const
{
    vFloat v(f);
    *this = v;
}

sfpi_inline vFloat vDReg::operator-() const
{
    vFloat tmp = *this;
    return __builtin_rvtt_sfpmov(tmp.get(), SFPMOV_MOD1_COMPSIGN);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat vConst::operator+(const vFloat b) const { return sfpi_int::fp_add(vFloat(*this), b); }
sfpi_inline vFloat vConst::operator-(const vFloat b) const { return sfpi_int::fp_sub(vFloat(*this), b); }
sfpi_inline vFloat vConst::operator*(const vFloat b) const { return sfpi_int::fp_mul(vFloat(*this), b); }
sfpi_inline vCondComp vConst::operator==(const vFloat x) const { return vCondComp(vCondComp::CompEQ, *this, x); }
sfpi_inline vCondComp vConst::operator!=(const vFloat x) const { return vCondComp(vCondComp::CompNE, *this, x); }
sfpi_inline vCondComp vConst::operator<(const vFloat x) const { return vCondComp(vCondComp::CompLT, *this, x); }
sfpi_inline vCondComp vConst::operator<=(const vFloat x) const { return vCondComp(vCondComp::CompLTE, vFloat(*this), x, true, false); }
sfpi_inline vCondComp vConst::operator>(const vFloat x) const { return vCondComp(vCondComp::CompGT, vFloat(*this), x, false, true); }
sfpi_inline vCondComp vConst::operator>=(const vFloat x) const { return vCondComp(vCondComp::CompGTE, *this, x); }

sfpi_inline vIntBase vConst::operator<<(uint32_t amt) const
{
    __rvtt_vec_t v = __builtin_rvtt_sfpassignlr(reg);
    return __builtin_rvtt_sfpshft_i(v, amt);
}

sfpi_inline vUInt vConst::operator>>(uint32_t amt) const
{
    __rvtt_vec_t v = __builtin_rvtt_sfpassignlr(reg);
    return __builtin_rvtt_sfpshft_i(v, -amt);
}

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
sfpi_inline void vBase::assign(const __rvtt_vec_t in)
{
    v = (initialized) ? __builtin_rvtt_sfpassign_lv(v, in) : in;
    initialized = true;
}

sfpi_inline void vBase::operator=(LRegAssignerInternal& lr)
{
    v = __builtin_rvtt_sfpassignlr(static_cast<std::underlying_type<LRegs>::type>(lr.lreg));
    lr.v = &v;
    initialized = true;
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat vFloat::operator+(const vFloat b) const { return sfpi_int::fp_add(*this, b); }
sfpi_inline vFloat vFloat::operator-(const vFloat b) const { return sfpi_int::fp_sub(*this, b); }
sfpi_inline vFloat vFloat::operator-(const float b) const { return sfpi_int::fp_add(*this, vFloat(-b)); }
sfpi_inline vFloat vFloat::operator-(const s2vFloat16 b) const { return sfpi_int::fp_add(*this, b.negate()); }
sfpi_inline vFloat vFloat::operator*(const vFloat b) const { return sfpi_int::fp_mul(*this, b); }
sfpi_inline vCondComp vFloat::operator==(const float x) const { return vCondComp(vCondComp::CompEQ, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator!=(const float x) const { return vCondComp(vCondComp::CompNE, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator<(const float x) const { return vCondComp(vCondComp::CompLT, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator<=(const float x) const { return vCondComp(vCondComp::CompLTE, *this, s2vFloat16(x), true, false); }
sfpi_inline vCondComp vFloat::operator>(const float x) const { return vCondComp(vCondComp::CompGT, *this, s2vFloat16(x), false, true); }
sfpi_inline vCondComp vFloat::operator>=(const float x) const { return vCondComp(vCondComp::CompGTE, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator==(const s2vFloat16 x) const { return vCondComp(vCondComp::CompEQ, *this, x); }
sfpi_inline vCondComp vFloat::operator!=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompNE, *this, x); }
sfpi_inline vCondComp vFloat::operator<(const s2vFloat16 x) const { return vCondComp(vCondComp::CompLT, *this, x); }
sfpi_inline vCondComp vFloat::operator<=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompLTE, *this, x, true, false); }
sfpi_inline vCondComp vFloat::operator>(const s2vFloat16 x) const { return vCondComp(vCondComp::CompGT, *this, x, false, true); }
sfpi_inline vCondComp vFloat::operator>=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompGTE, *this, x); }
sfpi_inline vCondComp vFloat::operator==(const vFloat x) const { return vCondComp(vCondComp::CompEQ, *this, x); }
sfpi_inline vCondComp vFloat::operator!=(const vFloat x) const { return vCondComp(vCondComp::CompNE, *this, x); }
sfpi_inline vCondComp vFloat::operator<(const vFloat x) const { return vCondComp(vCondComp::CompLT, *this, x); }
sfpi_inline vCondComp vFloat::operator<=(const vFloat x) const { return vCondComp(vCondComp::CompLTE, *this, x, true, false); }
sfpi_inline vCondComp vFloat::operator>(const vFloat x) const { return vCondComp(vCondComp::CompGT, *this, x, false, true); }
sfpi_inline vCondComp vFloat::operator>=(const vFloat x) const { return vCondComp(vCondComp::CompGTE, *this, x); }

sfpi_inline void vFloat::operator*=(const vFloat m)
{
    assign(__builtin_rvtt_sfpmul(v, m.get(), SFPMAD_MOD1_OFFSET_NONE));
}

sfpi_inline void vFloat::operator+=(const vFloat a)
{
    assign(__builtin_rvtt_sfpadd(v, a.get(), SFPMAD_MOD1_OFFSET_NONE));
}

sfpi_inline void vFloat::operator-=(const vFloat a)
{
    __rvtt_vec_t neg1 = __builtin_rvtt_sfpassignlr(vConstNeg1.get());
    assign(__builtin_rvtt_sfpmad(neg1, a.get(), v, SFPMAD_MOD1_OFFSET_NONE));
}

sfpi_inline vFloat::vFloat(const vDReg dreg)
{
    v = __builtin_rvtt_sfpload(SFPLOAD_MOD0_REBIAS_EXP, dreg.get());
    initialized = true;
}

sfpi_inline vFloat::vFloat(const vConst creg)
{
    v = __builtin_rvtt_sfpassignlr(creg.get());
    initialized = true;
}

sfpi_inline vFloat vFloat::operator-() const
{
    return __builtin_rvtt_sfpmov(v, SFPMOV_MOD1_COMPSIGN);
}

sfpi_inline void vFloat::loadf(const float val)
{
    s2vFloat16 c = s2vFloat16(val);
    assign(__builtin_rvtt_sfploadi_ex(c.get_format(), c.get()));
}

sfpi_inline void vFloat::loadf16(const s2vFloat16 val)
{
    assign(__builtin_rvtt_sfploadi_ex(val.get_format(), val.get()));
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void vIntBase::loadsi(int32_t val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_MOD0_SHORT, val));
}

sfpi_inline void vIntBase::loadui(uint32_t val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_MOD0_USHORT, val));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator&(const vType b) const
{
    return __builtin_rvtt_sfpand(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator&=(const vType b)
{
    v = __builtin_rvtt_sfpand(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator|(const vType b) const
{
    return __builtin_rvtt_sfpor(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator|=(const vType b)
{
    v = __builtin_rvtt_sfpor(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator^(const vType b) const
{
    __rvtt_vec_t tmp1, tmp2, ntmp2;

    tmp1 = __builtin_rvtt_sfpor(v, b.get());
    tmp2 = __builtin_rvtt_sfpand(v, b.get());
    ntmp2 = __builtin_rvtt_sfpnot(tmp2);

    return __builtin_rvtt_sfpand(tmp1, ntmp2);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator^=(const vType b)
{
    __rvtt_vec_t tmp1, tmp2, ntmp2;

    tmp1 = __builtin_rvtt_sfpor(v, b.get());
    tmp2 = __builtin_rvtt_sfpand(v, b.get());
    ntmp2 = __builtin_rvtt_sfpnot(tmp2);

    v = __builtin_rvtt_sfpand(tmp1, ntmp2);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator~() const
{
    return __builtin_rvtt_sfpnot(v);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator<<(uint32_t amt) const
{
    return __builtin_rvtt_sfpshft_i(v, amt);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator<<=(uint32_t amt)
{
    assign((static_cast<vType>(*this) << amt).get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::add(int32_t val, unsigned int mod_base) const
{
    return __builtin_rvtt_sfpiadd_i_ex(v, val, mod_base);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator+(const vIntBase val) const
{
    return __builtin_rvtt_sfpiadd_v_ex(val.get(), v, 0);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator+(const vConst val) const
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    return __builtin_rvtt_sfpiadd_v_ex(c, v, 0);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::sub(int32_t val, unsigned int mod_base) const
{
    return __builtin_rvtt_sfpiadd_i_ex(v, val, mod_base | SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator-(const vIntBase val) const
{
    return __builtin_rvtt_sfpiadd_v_ex(val.get(), v, SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator-(const vConst val) const
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    return __builtin_rvtt_sfpiadd_v_ex(c, v, SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::add_eq(int32_t val, unsigned int mod_base)
{
    assign(__builtin_rvtt_sfpiadd_i_ex(v, val, mod_base));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator+=(const vIntBase val)
{
    assign(__builtin_rvtt_sfpiadd_v_ex(v, val.get(), 0));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator+=(const vConst val)
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    assign(__builtin_rvtt_sfpiadd_v_ex(c, v, 0));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::sub_eq(int32_t val, unsigned int mod_base)
{
    assign(__builtin_rvtt_sfpiadd_i_ex(v, val, mod_base | SFPIADD_EX_MOD1_IS_SUB));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator-=(const vIntBase val)
{
    assign(__builtin_rvtt_sfpiadd_v_ex(val.get(), v, SFPIADD_EX_MOD1_IS_SUB));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator-=(const vConst val)
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    assign(__builtin_rvtt_sfpiadd_v_ex(c, v, SFPIADD_EX_MOD1_IS_SUB));
}

sfpi_inline const vCondOpLz vIntBase::lz_cc(const vBase src, LzCC cc)
{
    return vCondOpLz(this, src, cc);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline const vCondOpExExp vInt::exexp_cc(vFloat src, const ExExpCC cc) { return vCondOpExExp(this, src, SFPEXEXP_MOD1_DEBIAS, cc); }
sfpi_inline const vCondOpExExp vInt::exexp_nodebias_cc(vFloat src, const ExExpCC cc) { return vCondOpExExp(this, src, SFPEXEXP_MOD1_NODEBIAS, cc); }
sfpi_inline const vCondOpLz vInt::lz_cc(const vBase src, LzCC cc) { return vCondOpLz(this, src, cc); }
sfpi_inline const vCondOpIAddI vInt::add_cc(const vInt src, int32_t val, IAddCC cc) { return vCondOpIAddI(this, src, cc, val); }
sfpi_inline const vCondOpIAddV vInt::add_cc(const vInt src, IAddCC cc) { return vCondOpIAddV(this, src, cc); }

sfpi_inline const vCondComp vInt::operator==(int32_t val) const { return vCondComp(vCondComp::CompEQ, *this, val, SFPIADD_I_EX_MOD1_SIGNED); }
sfpi_inline const vCondComp vInt::operator!=(int32_t val) const { return vCondComp(vCondComp::CompNE, *this, val, SFPIADD_I_EX_MOD1_SIGNED); }
sfpi_inline const vCondComp vInt::operator<(int32_t val) const { return vCondComp(vCondComp::CompLT, *this, val, SFPIADD_I_EX_MOD1_SIGNED); }
sfpi_inline const vCondComp vInt::operator<=(int32_t val) const { return  vCondComp(vCondComp::CompLTE, *this, val, SFPIADD_I_EX_MOD1_SIGNED, true, false); }
sfpi_inline const vCondComp vInt::operator>(int32_t val) const { return  vCondComp(vCondComp::CompGT, *this, val, SFPIADD_I_EX_MOD1_SIGNED, false, true); }
sfpi_inline const vCondComp vInt::operator>=(int32_t val) const { return vCondComp(vCondComp::CompGTE, *this, val, SFPIADD_I_EX_MOD1_SIGNED); }

sfpi_inline const vCondComp vInt::operator==(const vIntBase src) const { return vCondComp(vCondComp::CompEQ, *this, src, 0); }
sfpi_inline const vCondComp vInt::operator!=(const vIntBase src) const { return vCondComp(vCondComp::CompNE, *this, src, 0); }
sfpi_inline const vCondComp vInt::operator<(const vIntBase src) const { return vCondComp(vCondComp::CompLT, *this, src, 0); }
sfpi_inline const vCondComp vInt::operator<=(const vIntBase src) const { return vCondComp(vCondComp::CompLTE, *this, src, 0, true, false); }
sfpi_inline const vCondComp vInt::operator>(const vIntBase src) const { return vCondComp(vCondComp::CompGT, *this, src, 0, false, true); }
sfpi_inline const vCondComp vInt::operator>=(const vIntBase src) const { return vCondComp(vCondComp::CompGTE, *this, src, 0); }

//////////////////////////////////////////////////////////////////////////////
sfpi_inline const vCondOpIAddI vUInt::add_cc(const vUInt src, int32_t val, IAddCC cc) { return vCondOpIAddI(this, src, cc, val); }
sfpi_inline const vCondOpIAddV vUInt::add_cc(const vUInt src, IAddCC cc) { return vCondOpIAddV(this, src, cc); }

sfpi_inline const vCondComp vUInt::operator==(int32_t val) const { return vCondComp(vCondComp::CompEQ, *this, val, 0); }
sfpi_inline const vCondComp vUInt::operator!=(int32_t val) const { return vCondComp(vCondComp::CompNE, *this, val, 0); }
sfpi_inline const vCondComp vUInt::operator<(int32_t val) const { return vCondComp(vCondComp::CompLT, *this, val, 0); }
sfpi_inline const vCondComp vUInt::operator<=(int32_t val) const { return  vCondComp(vCondComp::CompLTE, *this, val, 0, true, false); }
sfpi_inline const vCondComp vUInt::operator>(int32_t val) const { return  vCondComp(vCondComp::CompGT, *this, val, 0, false, true); }
sfpi_inline const vCondComp vUInt::operator>=(int32_t val) const { return vCondComp(vCondComp::CompGTE, *this, val, 0); }

sfpi_inline const vCondComp vUInt::operator==(const vIntBase src) const { return vCondComp(vCondComp::CompEQ, *this, src, 0); }
sfpi_inline const vCondComp vUInt::operator!=(const vIntBase src) const { return vCondComp(vCondComp::CompNE, *this, src, 0); }
sfpi_inline const vCondComp vUInt::operator<(const vIntBase src) const { return vCondComp(vCondComp::CompLT, *this, src, 0); }
sfpi_inline const vCondComp vUInt::operator<=(const vIntBase src) const { return vCondComp(vCondComp::CompLTE, *this, src, 0, true, false); }
sfpi_inline const vCondComp vUInt::operator>(const vIntBase src) const { return vCondComp(vCondComp::CompGT, *this, src, 0, false, true); }
sfpi_inline const vCondComp vUInt::operator>=(const vIntBase src) const { return vCondComp(vCondComp::CompGTE, *this, src, 0); }

sfpi_inline vUInt vUInt::operator>>(uint32_t amt) const
{
    return __builtin_rvtt_sfpshft_i(v, -amt);
}

sfpi_inline void vUInt::operator>>=(uint32_t amt)
{
    assign((*this >> amt).get());
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void vCondOperand::emit(vCCCtrl *cc) const
{
    if (type == Type::vCond) {
        op->emit(false);
    } else {
        cond->emit(cc);
    }
}

sfpi_inline const vBool vBool::operator!() const
{
    vBool result(*this);
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
//
// Descending the LHS uses the last PUSHC as the "gate" against which a COMPC
// can be issued, however, descending the RHS would mess up the results from
// the LHS w/o a new gate, hence the PUSHC prior to the RHS.  The POPC would
// destroy the results of the RHS and so those results are saved/restored with
// saved_enables.  This uses 1 reg per level for now, which could be optimized
// to just 1 reg total w/ some compiler work to clean up unused LOADIs that
// are set multiple times to the same register (this is a tradeoff of
// imperfect performance for what is likely a rare case anyway).
#define __sfpi_cond_emit_loop(leftside, rightside)                      \
{                                                                       \
    const vBool* local_node = node;                                   \
    bool negate_node = node->negate;                                    \
    bool negate_child = negate;                                         \
    bool descended_right = false;                                       \
    vInt saved_enables = 1;                                         \
                                                                        \
    vBool::vBoolType node_type = negate ? node->get_neg_type() : node->get_type(); \
    if (node_type == vBool::vBoolType::Or) {                        \
        negate_node = !negate_node;                                     \
        negate_child = !negate_child;                                   \
    }                                                                   \
                                                                        \
    if (node->op_a.get_type() == vCondOperand::Type::vBool) {          \
        node = &node->op_a.get_cond();                                  \
        leftside;                                                       \
        node = local_node;                                              \
    } else {                                                            \
        node->op_a.get_op().emit(negate_child);                         \
    }                                                                   \
                                                                        \
    if (node->op_b.get_type() == vCondOperand::Type::vBool) {          \
        cc->push();                                                     \
        node = &node->op_b.get_cond();                                  \
        rightside;                                                      \
        descended_right = true;                                         \
        node = local_node;                                              \
    } else {                                                            \
        bool restore = false;                                           \
        if (node->op_b.get_op().issues_compc(negate_child)) {           \
            restore = true;                                             \
            cc->push();                                                 \
        }                                                               \
        node->op_b.get_op().emit(negate_child);                         \
        if (restore) {                                                  \
            saved_enables = 0;                                          \
            cc->pop();                                                  \
            __builtin_rvtt_sfpsetcc_v(saved_enables.get(), SFPSETCC_MOD1_LREG_EQ0); \
        }                                                               \
    }                                                                   \
                                                                        \
    if (descended_right) {                                              \
        saved_enables = 0;                                              \
        cc->pop();                                                      \
        __builtin_rvtt_sfpsetcc_v(saved_enables.get(), SFPSETCC_MOD1_LREG_EQ0); \
    }                                                                   \
    if (negate_node) {                                                  \
        __builtin_rvtt_sfpcompc();                                      \
    }                                                                   \
}

#define __sfpi_cond_emit_loop_mid(leftside, rightside)                  \
    bool negate = negate_child;                                         \
    __sfpi_cond_emit_loop(leftside, rightside)

#define __sfpi_cond_emit_error __builtin_rvtt_sfpillegal();
sfpi_inline void vBool::emit(vCCCtrl *cc, bool negate) const
{
    const vBool* node = this;

    __sfpi_cond_emit_loop(
        __sfpi_cond_emit_loop_mid(__sfpi_cond_emit_loop_mid(__sfpi_cond_emit_error, __sfpi_cond_emit_error),
                                  __sfpi_cond_emit_loop_mid(__sfpi_cond_emit_error, __sfpi_cond_emit_error)),
        __sfpi_cond_emit_loop_mid(__sfpi_cond_emit_loop_mid(__sfpi_cond_emit_error, __sfpi_cond_emit_error),
                                  __sfpi_cond_emit_loop_mid(__sfpi_cond_emit_error, __sfpi_cond_emit_error)));
}

sfpi_inline vCond::vCond(vCondOpType t, const vInt a, int32_t i, uint32_t m, uint32_t nm, bool c, bool nc) :
 type(t), op_a(a), op_b(), imm(i), mod1(m), neg_mod1(nm), comp(c), neg_comp(nc)
{
}

sfpi_inline const vCond vCond::operator!() const
{
    vCond result(*this);

    result.mod1 = neg_mod1;
    result.neg_mod1 = mod1;

    return result;
}

sfpi_inline void vCond::emit(bool negate) const
{
    uint32_t use_mod1 = negate ? neg_mod1 : mod1;

    switch (type) {
    case vCond::vCondOpType::CompareFloat:
        __builtin_rvtt_sfpscmp_ex(op_a.get_vec().get(), op_b.get_scalarfp().get(),
                                  use_mod1 | (op_b.get_scalarfp().get_format() ==
                                              SFPLOADI_MOD0_FLOATA ? SFPSCMP_EX_MOD1_FMT_A : 0));
        break;

    case vCond::vCondOpType::ComparevFloat:
        __builtin_rvtt_sfpvcmp_ex(op_a.get_vec().get(), op_b.get_vec().get(), use_mod1);
        break;

    case vCond::vCondOpType::CompareShort:
        __builtin_rvtt_sfpiadd_i_ex(op_a.get_vec().get(), imm, use_mod1 | SFPIADD_EX_MOD1_IS_SUB);
        break;

    case vCond::vCondOpType::ComparevInt:
        // Remember: dst gets negated
        __builtin_rvtt_sfpiadd_v_ex(op_b.get_vec().get(), op_a.get_vec().get(),
                                    use_mod1 | SFPIADD_EX_MOD1_IS_SUB);
        break;

    case vCond::vCondOpType::ExExp:
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfpexexp(op_a.get_vec().get(), use_mod1);
        break;

    case vCond::vCondOpType::Lz:
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfplz(op_a.get_vec().get(), use_mod1);
        break;

    case vCond::vCondOpType::IAddI:
        // This is legacy code for add_cc implementation
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfpiadd_i(imm, op_a.get_vec().get(), use_mod1);
        break;

    case vCond::vCondOpType::IAddV:
        // This is legacy code for add_cc implementation
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfpiadd_v(op_b.get_vec_ptr()->get(),
                                                             op_a.get_vec().get(),
                                                             use_mod1);
        break;
    }
}

sfpi_inline vCondComp::vCondCompOpType vCondComp::not_cond(const vCondCompOpType t) const
{
    switch (t) {
    case CompLT:
        return CompGTE;
    case CompNE:
        return CompEQ;
    case CompGTE:
        return CompLT;
    case CompEQ:
        return CompNE;
    case CompLTE:
        return CompGT;
    case CompGT:
        return CompLTE;
    default:
        // Should never get here
        return CompNE;
    }
}

sfpi_inline ExExpCC vCondOpExExp::not_cond(const ExExpCC t) const
{
    return (t == ExExpCCLT0) ? ExExpCCGTE0 : ExExpCCLT0;
}

sfpi_inline LzCC vCondOpLz::not_cond(const LzCC t) const
{
    return (t == LzCCNE0) ? LzCCEQ0 : LzCCNE0;
}

sfpi_inline IAddCC vCondOpIAddI::not_cond(const IAddCC t) const
{
    return (t == IAddCCLT0) ? IAddCCGTE0 : IAddCCLT0;
}

sfpi_inline IAddCC vCondOpIAddV::not_cond(const IAddCC t) const
{
    return (t == IAddCCLT0) ? IAddCCGTE0 : IAddCCLT0;
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vCCCtrl::vCCCtrl() : push_count(0)
{
    push();
}

sfpi_inline void vCCCtrl::cc_if(const vBool& op)
{
    op.emit(this);
}

sfpi_inline void vCCCtrl::cc_if(const vCond& op)
{
    op.emit(false);
}

sfpi_inline void vCCCtrl::cc_elseif(const vBool& op)
{
    cc_if(op);
}

sfpi_inline void vCCCtrl::cc_elseif(const vCond& op)
{
    cc_if(op);
}

sfpi_inline void vCCCtrl::cc_else() const
{
    __builtin_rvtt_sfpcompc();
}

sfpi_inline vCCCtrl::~vCCCtrl()
{
    while (push_count != 0) {
        pop();
    }
}

sfpi_inline void vCCCtrl::push()
{
    push_count++;
    __builtin_rvtt_sfppushc();
}

sfpi_inline void vCCCtrl::pop()
{
    push_count--;
    __builtin_rvtt_sfppopc();
}

sfpi_inline void vCCCtrl::enable_cc()
{
    __builtin_rvtt_sfpencc(SFPENCC_IMM12_BOTH, SFPENCC_MOD1_EI_RI);
}

//////////////////////////////////////////////////////////////////////////////
// Functional math library
//////////////////////////////////////////////////////////////////////////////
 sfpi_inline vFloat lut(const vFloat v, const vUInt l0, const vUInt l1, const vUInt l2, const int offset = 0)
{
    unsigned int bias_mask = (offset == 1) ? SFPLUT_MOD0_BIAS_POS :
        ((offset == -1) ? SFPLUT_MOD0_BIAS_NEG : SFPLUT_MOD0_BIAS_NONE);
    return __builtin_rvtt_sfplut(l0.get(), l1.get(), l2.get(), v.get(), SFPLUT_MOD0_SGN_RETAIN | bias_mask);
}

sfpi_inline vFloat lut_sign(const vFloat v, const vUInt l0, const vUInt l1, const vUInt l2, const int offset = 0)
{
    unsigned int bias_mask = (offset == 1) ? SFPLUT_MOD0_BIAS_POS :
        ((offset == -1) ? SFPLUT_MOD0_BIAS_NEG : SFPLUT_MOD0_BIAS_NONE);
    return __builtin_rvtt_sfplut(l0.get(), l1.get(), l2.get(), v.get(), SFPLUT_MOD0_SGN_UPDATE | bias_mask);
}

sfpi_inline vInt exexp(const vFloat v)
{
    return __builtin_rvtt_sfpexexp(v.get(), SFPEXEXP_MOD1_DEBIAS);
}

sfpi_inline vInt exexp_nodebias(const vFloat v)
{
    return __builtin_rvtt_sfpexexp(v.get(), SFPEXEXP_MOD1_NODEBIAS);
}

sfpi_inline vInt exman8(const vFloat v)
{
    return __builtin_rvtt_sfpexman(v.get(), SFPEXMAN_MOD1_PAD8);
}

sfpi_inline vInt exman9(const vFloat v)
{
    return __builtin_rvtt_sfpexman(v.get(), SFPEXMAN_MOD1_PAD9);
}

sfpi_inline vFloat setexp(const vFloat v, const uint32_t exp)
{
    return __builtin_rvtt_sfpsetexp_i(exp, v.get());
}

sfpi_inline vFloat setexp(const vFloat v, const vIntBase exp)
{
    // Odd: dst is both exponent and result so undergoes a type change
    // If exp is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetexp_v(exp.get(), v.get());
}

sfpi_inline vFloat setman(const vFloat v, const uint32_t man)
{
    return __builtin_rvtt_sfpsetman_i(man, v.get());
}

sfpi_inline vFloat setman(const vFloat v, const vIntBase man)
{
    // Grayskull HW bug, is this useful?  Should there be a "Half" form?
    // Odd: dst is both man and result so undergoes a type change
    // If man is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetman_v(man.get(), v.get());
}

sfpi_inline vFloat addexp(const vFloat in, const int32_t exp)
{
    return __builtin_rvtt_sfpdivp2(exp, in.get(), SFPSDIVP2_MOD1_ADD);
}

sfpi_inline vFloat setsgn(const vFloat v, const int32_t sgn)
{
    return __builtin_rvtt_sfpsetsgn_i(sgn, v.get());
}

sfpi_inline vFloat setsgn(const vFloat v, const vFloat sgn)
{
    // Odd: dst is both sgn and result so undergoes a type change
    // If sgn is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetsgn_v(sgn.get(), v.get());
}

sfpi_inline vFloat setsgn(const vFloat v, const vInt sgn)
{
    // Odd: dst is both sgn and result so undergoes a type change
    // If sgn is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetsgn_v(sgn.get(), v.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vBase, vType>::value>* = nullptr>
sfpi_inline vInt lz(const vType v)
{
    return vInt(__builtin_rvtt_sfplz(v.get(), SFPLZ_MOD1_CC_NONE));
}

sfpi_inline vFloat abs(const vFloat v)
{
    return __builtin_rvtt_sfpabs(v.get(), SFPABS_MOD1_FLOAT);
}

sfpi_inline vInt abs(const vInt v)
{
    return __builtin_rvtt_sfpabs(v.get(), SFPABS_MOD1_INT);
}

sfpi_inline vUInt shft(const vUInt v, const vInt amt)
{
    return __builtin_rvtt_sfpshft_v(v.get(), amt.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vBase, vType>::value>* = nullptr>
sfpi_inline vType reinterpret(const vBase v)
{
    return vType(v.get());
}

} // namespace sfpi


#define v_if(x)             \
{                           \
    vCCCtrl __cc;            \
    __cc.cc_if(x);

#define v_elseif(x)         \
    __cc.cc_else();         \
    __cc.push();            \
    __cc.cc_elseif(x);

#define v_else              \
    __cc.cc_else();

#define v_endif             \
}

#define v_block             \
{                           \
    vCCCtrl __cc;

#define v_and(x)            \
    __cc.cc_if(x)

#define p_endblock          \
}
