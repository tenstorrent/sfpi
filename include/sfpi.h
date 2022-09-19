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
//   class __vDReg
//   constexpr __RegFile<__vDReg, 64> dst_reg;
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
//   class __vConstFloat
//   constexpr __vConstFloat vConst0p6928(5);
//
//   The constant value local registers are used in expressions by referencing
//   one of the names above and using them in mathematical operations such as:
//       a = b * c + vConst0p6928;
//
// Predicated Execution:
//   class __vCCCtrl
//   macros v_if(), v_elseif(), v_else, v_endif
//
//   The class __vCCCtrl is used in conjunction with the LReg based classes to
//   enable predicated execution.  By convention the test infastructure indents
//   the code as if executing if/then/else in C++, for example:
//     __vCCCtrl cc;
//     vFloat v = dst_reg[0];
//     cc.cc_if(v < 5.0F); {
//         // if side
//     } cc.cc_else(); {
//         // else side
//     }
//     cc.cc_endif();
//   The above chatter is reduced w/ use of macros listed above.  The macros
//   also balance {} to auto-destruct the __vCCCtrl when appropriate.
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

#if defined(ARCH_WORMHOLE_B0)
#define ARCH_WORMHOLE 1
#endif

#if defined(ARCH_GRAYSKULL)
#include <grayskull/sfpi_hw.h>
#elif defined(ARCH_WORMHOLE)
#include <wormhole/sfpi_hw.h>
#endif

#include <sfpi_fp16.h>

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
// Interface
class vFloat;
class vInt;
class vUInt;
enum class LRegs;

// Internal
class __vBase;
class __vIntBase;
class __vConstFloat;
class __vConstIntBase;
class __vCond;
class __vCCCtrl;
class __vCCCtrlBase;

//////////////////////////////////////////////////////////////////////////////
sfpi_inline unsigned int __f32asui(const float val)
{
    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const float inf) : f(inf) {}
    } tmp(val);

    return tmp.i;
}

//////////////////////////////////////////////////////////////////////////////
template<class Type, int N>
class __RegFile {

public:
    sfpi_inline Type operator[](const int x) const;
};

//////////////////////////////////////////////////////////////////////////////
class __vRegBase {
protected:
    int reg;

public:
    constexpr explicit __vRegBase(int r) : reg(r) {}
    constexpr int get() const { return reg; }
};

//////////////////////////////////////////////////////////////////////////////
class __vRegBaseInitializer {
    int n;

 public:
    constexpr __vRegBaseInitializer(int in) : n(in) {}
    constexpr int get() const { return n; }
};

//////////////////////////////////////////////////////////////////////////////
class __vDReg : public __vRegBase {
private:
    template<class Type, int N> friend class __RegFile;
    constexpr explicit __vDReg(const __vRegBaseInitializer i) : __vRegBase(i.get() * SFP_DESTREG_STRIDE) {}

public:
    // Assign register to register
    template <typename vecType, typename std::enable_if_t<std::is_base_of<__vBase, vecType>::value>* = nullptr>
    sfpi_inline vecType operator=(const vecType vec) const;
    sfpi_inline void operator=(const __vDReg dreg) const;
    sfpi_inline vFloat operator=(const __vConstFloat creg) const;
    sfpi_inline vFloat operator=(const s2vFloat16 f) const;
    sfpi_inline vInt operator=(const int i) const;
    sfpi_inline vUInt operator=(const unsigned int i) const;
    sfpi_inline vFloat operator=(const float f) const;
    sfpi_inline vFloat operator=(const double d) const;

    // Construct operator classes from operations
    sfpi_inline vFloat operator+(const vFloat b) const;
    sfpi_inline vFloat operator-(const vFloat b) const;
    sfpi_inline vFloat operator-() const;
    sfpi_inline vFloat operator*(const vFloat b) const;

    // Conditionals
    sfpi_inline __vCond operator==(const float x) const;
    sfpi_inline __vCond operator!=(const float x) const;
    sfpi_inline __vCond operator<(const float x) const;
    sfpi_inline __vCond operator<=(const float x) const;
    sfpi_inline __vCond operator>(const float x) const;
    sfpi_inline __vCond operator>=(const float x) const;

    sfpi_inline __vCond operator==(const s2vFloat16 x) const;
    sfpi_inline __vCond operator!=(const s2vFloat16 x) const;
    sfpi_inline __vCond operator<(const s2vFloat16 x) const;
    sfpi_inline __vCond operator<=(const s2vFloat16 x) const;
    sfpi_inline __vCond operator>(const s2vFloat16 x) const;
    sfpi_inline __vCond operator>=(const s2vFloat16 x) const;

    sfpi_inline __vCond operator==(const vFloat x) const;
    sfpi_inline __vCond operator!=(const vFloat x) const;
    sfpi_inline __vCond operator<(const vFloat x) const;
    sfpi_inline __vCond operator<=(const vFloat x) const;
    sfpi_inline __vCond operator>(const vFloat x) const;
    sfpi_inline __vCond operator>=(const vFloat x) const;
};

//////////////////////////////////////////////////////////////////////////////
class __DestReg {
 private:
    __RegFile<__vDReg, SFP_DESTREG_MAX_ADDR> dreg;

 public:
    sfpi_inline const __vDReg operator[](const int i) const { return dreg[i]; }
    // Make these void - ugly as these aren't really inc/dec
    sfpi_inline void operator++() const { __builtin_rvtt_sfpincrwc(0, SFP_DESTREG_STRIDE, 0, 0); }
    sfpi_inline void operator++(const int) const { __builtin_rvtt_sfpincrwc(0, SFP_DESTREG_STRIDE, 0, 0); }
    sfpi_inline void operator+=(const int i) const { __builtin_rvtt_sfpincrwc(0, SFP_DESTREG_STRIDE * i, 0, 0); }
};

//////////////////////////////////////////////////////////////////////////////
class __vLReg : public __vRegBase {
 private:
    template<class Type, int N> friend class __RegFile;
    constexpr explicit __vLReg(const __vRegBaseInitializer i) : __vRegBase(i.get()) {}

 public:
    sfpi_inline __rvtt_vec_t operator=(__vBase& v) const;
};

class __LReg {
 private:
    __RegFile<__vLReg, SFP_LREG_COUNT> lreg;

 public:
    sfpi_inline const __vLReg operator[](const enum LRegs lr) const { return lreg[static_cast<std::underlying_type<LRegs>::type>(lr)]; }
};

//////////////////////////////////////////////////////////////////////////////
class __vConstFloat : public __vRegBase {
public:
    constexpr explicit __vConstFloat(int r) : __vRegBase(r) {}

#ifdef ARCH_WORMHOLE
    sfpi_inline void operator=(const float in) const;
    sfpi_inline void operator=(const s2vFloat16 in) const;
#endif

    // Construct operator classes from operations
    sfpi_inline vFloat operator+(const vFloat b) const;
    sfpi_inline vFloat operator-(const vFloat b) const;
    sfpi_inline vFloat operator*(const vFloat b) const;

    sfpi_inline __vCond operator==(const vFloat x) const;
    sfpi_inline __vCond operator!=(const vFloat x) const;
    sfpi_inline __vCond operator<(const vFloat x) const;
    sfpi_inline __vCond operator<=(const vFloat x) const;
    sfpi_inline __vCond operator>(const vFloat x) const;
    sfpi_inline __vCond operator>=(const vFloat x) const;
};

class __vConstIntBase : public __vRegBase {
public:
    constexpr explicit __vConstIntBase(int r) : __vRegBase(r) {}

#ifdef ARCH_WORMHOLE
    sfpi_inline void operator=(const int in) const;
#endif

    // Construct operator classes from operations
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator+(const vType b) const;
    sfpi_inline vInt operator+(int32_t b) const;

    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator-(const vType b) const;
    sfpi_inline vInt operator-(int32_t b) const;

    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator&(const vType b) const;
    sfpi_inline vInt operator&(int32_t b) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator|(const vType b) const;
    sfpi_inline vInt operator|(int32_t b) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator^(const vType b) const;
    sfpi_inline vInt operator^(int32_t b) const;

    sfpi_inline __vCond operator==(const vInt x) const;
    sfpi_inline __vCond operator!=(const vInt x) const;
    sfpi_inline __vCond operator<(const vInt x) const;
    sfpi_inline __vCond operator<=(const vInt x) const;
    sfpi_inline __vCond operator>(const vInt x) const;
    sfpi_inline __vCond operator>=(const vInt x) const;

    // Shifts
    sfpi_inline __vIntBase operator<<(uint32_t amt) const;
    sfpi_inline vUInt operator>>(uint32_t amt) const;
};

//////////////////////////////////////////////////////////////////////////////
class __vBase {
protected:
    bool initialized;
    __rvtt_vec_t v;

    sfpi_inline void assign(const __rvtt_vec_t t);

public:
    sfpi_inline __vBase() : initialized(false) {}

    sfpi_inline __rvtt_vec_t get() const { return v; }
    sfpi_inline __rvtt_vec_t& get() { return v; }

    // Associate variable w/ a value pre-loaded into a particular lreg
    sfpi_inline void operator=(__vLReg lr);
};

//////////////////////////////////////////////////////////////////////////////
class vFloat : public __vBase {
private:
    sfpi_inline void loadf(const float val);
    sfpi_inline void loadf16(const s2vFloat16 val);

public:
    vFloat() = default;

    sfpi_inline vFloat(const __vDReg dreg);
    sfpi_inline vFloat(const __vConstFloat creg);
    sfpi_inline vFloat(const s2vFloat16 f) { loadf16(f); }
    sfpi_inline vFloat(const float f) { loadf(f); }
    sfpi_inline vFloat(const __rvtt_vec_t& t) { assign(t); }
    sfpi_inline vFloat(__vLReg lr) { __vBase::operator=(lr); }

    // Assignment
    sfpi_inline vFloat operator=(const vFloat in) { assign(in.v); return v; }
    sfpi_inline vFloat operator=(__vLReg lr) { __vBase::operator=(lr); return v; }

    // Construct operator from operations
    sfpi_inline vFloat operator+(const vFloat b) const;
    sfpi_inline vFloat operator+=(const vFloat);
    sfpi_inline vFloat operator-(const vFloat b) const;
    sfpi_inline vFloat operator-(const float b) const;
    sfpi_inline vFloat operator-(const s2vFloat16 b) const;
    sfpi_inline vFloat operator-=(const vFloat);
    sfpi_inline vFloat operator-() const;
    sfpi_inline vFloat operator*(const vFloat b) const;
    sfpi_inline vFloat operator*=(const vFloat);
    sfpi_inline vFloat operator++(const int) { *this += 1; return *this; }
    sfpi_inline vFloat operator++() { vFloat tmp = *this; *this += 1; return tmp; }
    sfpi_inline vFloat operator--(const int) { *this -= 1; return *this; }
    sfpi_inline vFloat operator--() { vFloat tmp = *this; *this -= 1; return tmp; }

    // Conditionals
    sfpi_inline __vCond operator==(const float x) const;
    sfpi_inline __vCond operator==(const s2vFloat16 x) const;
    sfpi_inline __vCond operator==(const vFloat x) const;
    sfpi_inline __vCond operator!=(const float x) const;
    sfpi_inline __vCond operator!=(const s2vFloat16 x) const;
    sfpi_inline __vCond operator!=(const vFloat x) const;
    sfpi_inline __vCond operator<(const float x) const;
    sfpi_inline __vCond operator<(const s2vFloat16 x) const;
    sfpi_inline __vCond operator<(const vFloat x) const;
    sfpi_inline __vCond operator<=(const float x) const;
    sfpi_inline __vCond operator<=(const s2vFloat16 x) const;
    sfpi_inline __vCond operator<=(const vFloat x) const;
    sfpi_inline __vCond operator>(const float x) const;
    sfpi_inline __vCond operator>(const s2vFloat16 x) const;
    sfpi_inline __vCond operator>(const vFloat x) const;
    sfpi_inline __vCond operator>=(const float x) const;
    sfpi_inline __vCond operator>=(const s2vFloat16 x) const;
    sfpi_inline __vCond operator>=(const vFloat x) const;
};

//////////////////////////////////////////////////////////////////////////////
class __vIntBase : public __vBase {
 protected:
    sfpi_inline void loadss(int16_t val);
    sfpi_inline void loadus(uint16_t val);
    sfpi_inline void loadsi(int32_t val);
    sfpi_inline void loadui(uint32_t val);

 public:
    __vIntBase() = default;
    sfpi_inline __vIntBase(const __rvtt_vec_t& in) { assign(in); }
    sfpi_inline __vIntBase(const __vConstIntBase creg);
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vBase, vType>::value>* = nullptr>
    sfpi_inline explicit operator vType() const { return vType(v); }

    // Bit Operations
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator&(const vType b) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline void operator&=(const vType b);
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator|(const vType b) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator|=(const vType b);
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator^(const vType b) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator^=(const vType b);
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator~() const;

    // Arith
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType add(int32_t val, unsigned int mod) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator+(const __vIntBase val) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator+(const __vConstIntBase val) const;

    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType sub(int32_t val, unsigned int mod) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator-(const __vIntBase val) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator-(const __vConstIntBase val) const;

    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline void add_eq(int32_t val, unsigned int mod);
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator+=(const __vIntBase val);
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator+=(const __vConstIntBase val);

    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline void sub_eq(int32_t val, unsigned int mod);
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator-=(const __vIntBase val);
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator-=(const __vConstIntBase val);

    // Shifts
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator<<(uint32_t amt) const;
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vType operator<<=(uint32_t amt);

};

class vInt : public __vIntBase {
    friend class vUInt;

public:
    vInt() = default;
    sfpi_inline vInt(const __rvtt_vec_t& in) { assign(in); }
    sfpi_inline vInt(const __vConstIntBase creg) { v = __builtin_rvtt_sfpassignlreg(creg.get()); initialized = true; }
    sfpi_inline vInt(const __vIntBase in) { assign(in.get()); };
    sfpi_inline vInt(short val) { loadss(val); }
    sfpi_inline vInt(int val) { loadsi(val); }
#ifndef __clang__
    sfpi_inline vInt(int32_t val) { loadsi(val); }
#endif
    sfpi_inline vInt(unsigned short val) { loadus(val); }
    sfpi_inline vInt(unsigned int val) { loadui(val); }
#ifndef __clang__
    sfpi_inline vInt(uint32_t val) { loadui(val); }
#endif
    sfpi_inline vInt(__vLReg lr) { __vBase::operator=(lr); }
    sfpi_inline vInt(const __vCond vc);

    // Assignment
    sfpi_inline vInt operator=(const vInt in) { assign(in.v); return v; }
    sfpi_inline vInt operator=(__vLReg lr) { __vBase::operator=(lr); return v; }

    // Operations
    sfpi_inline vInt operator&(int32_t b) const { return this->__vIntBase::operator&(vInt(b)); }
    sfpi_inline vInt operator&(const vInt b) const { return this->__vIntBase::operator&(b); }
    sfpi_inline vInt operator&=(const vInt b) { this->__vIntBase::operator&=(b); return v; }
    sfpi_inline vInt operator|(int32_t b) const { return this->__vIntBase::operator|(vInt(b)); }
    sfpi_inline vInt operator|(const vInt b) const { return this->__vIntBase::operator|(b); }
    sfpi_inline vInt operator|=(const vInt b) { this->__vIntBase::operator|=(b); return v; }
    sfpi_inline vInt operator^(int32_t b) const { return this->__vIntBase::operator^(vInt(b)); }
    sfpi_inline vInt operator^(const vInt b) const { return this->__vIntBase::operator^(b); }
    sfpi_inline vInt operator^=(const vInt b) { this->__vIntBase::operator^=(b); return v; }
    sfpi_inline vInt operator~() const { return this->__vIntBase::operator~<vInt>(); }

    sfpi_inline vInt operator+(int32_t val) const { return this->__vIntBase::add<vInt>(val, SFPXIADD_MOD1_SIGNED); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vInt operator+(const vType val) const { return this->__vIntBase::operator+<vInt>(val); }
    sfpi_inline vInt operator+(const __vConstIntBase val) const { return this->__vIntBase::operator+<vInt>(val); }

    sfpi_inline vInt operator-(int32_t val) const { return this->__vIntBase::sub<vInt>(val, SFPXIADD_MOD1_SIGNED); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vInt operator-(const vType val) const { return this->__vIntBase::operator-<vInt>(val); }
    sfpi_inline vInt operator-(const __vConstIntBase val) const { return this->__vIntBase::operator-<vInt>(val); }

    sfpi_inline vInt operator+=(int32_t val) { this->__vIntBase::add_eq<vInt>(val, SFPXIADD_MOD1_SIGNED); return v; }
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vInt operator+=(const vType val) { this->__vIntBase::operator+=<vInt>(val); return v; }
    sfpi_inline vInt operator+=(const __vConstIntBase val) { this->__vIntBase::operator-=<vInt>(val); return v; }

    sfpi_inline vInt operator-=(int32_t val) { this->__vIntBase::sub_eq<vInt>(val, SFPXIADD_MOD1_SIGNED); return v; }
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vInt operator-=(const vType val) { this->__vIntBase::operator-=<vInt>(val); return v; }
    sfpi_inline vInt operator-=(const __vConstIntBase val) { this->__vIntBase::operator-=<vInt>(val); return v; }

    sfpi_inline vInt operator++(const int) { *this += 1; return *this; }
    sfpi_inline vInt operator++() { vInt tmp = *this; *this += 1; return tmp; }
    sfpi_inline vInt operator--(const int) { *this -= 1; return *this; }
    sfpi_inline vInt operator--() { vInt tmp = *this; *this -= 1; return tmp; }

    sfpi_inline vInt operator<<(uint32_t amt) const { return this->__vIntBase::operator<<<vInt>(amt); }
    sfpi_inline vInt operator<<=(uint32_t amt) { this->__vIntBase::operator<<=<vInt>(amt); return v; }

    // Conditionals
    sfpi_inline const __vCond operator==(int32_t val) const;
    sfpi_inline const __vCond operator!=(int32_t val) const;
    sfpi_inline const __vCond operator<(int32_t val) const;
    sfpi_inline const __vCond operator<=(int32_t val) const;
    sfpi_inline const __vCond operator>(int32_t val) const;
    sfpi_inline const __vCond operator>=(int32_t val) const;

    sfpi_inline const __vCond operator==(const __vIntBase src) const;
    sfpi_inline const __vCond operator!=(const __vIntBase src) const;
    sfpi_inline const __vCond operator<(const __vIntBase src) const;
    sfpi_inline const __vCond operator<=(const __vIntBase src) const;
    sfpi_inline const __vCond operator>(const __vIntBase src) const;
    sfpi_inline const __vCond operator>=(const __vIntBase src) const;
};

//////////////////////////////////////////////////////////////////////////////
class vUInt : public __vIntBase {
    friend class vInt;

private:

public:
    vUInt() = default;
    sfpi_inline vUInt(const __rvtt_vec_t& in) { assign(in); }
    sfpi_inline vUInt(const __vConstIntBase creg) { v = __builtin_rvtt_sfpassignlreg(creg.get()); initialized = true; }
    sfpi_inline vUInt(const __vIntBase in) { assign(in.get()); }
    sfpi_inline vUInt(short val) { loadss(val); }
    sfpi_inline vUInt(int val) { loadsi(val); }
#ifndef __clang__
    sfpi_inline vUInt(int32_t val) { loadsi(val); }
#endif
    sfpi_inline vUInt(unsigned short val) { loadus(val); }
    sfpi_inline vUInt(unsigned int val) { loadui(val); }
#ifndef __clang__
    sfpi_inline vUInt(uint32_t val) { loadui(val); }
#endif
    sfpi_inline vUInt(__vLReg lr) { __vBase::operator=(lr); }
    sfpi_inline vUInt(const __vCond vc);

    // Assignment
    sfpi_inline vUInt operator=(const vUInt in ) { assign(in.v); return v; }
    sfpi_inline vUInt operator=(__vLReg lr) { __vBase::operator=(lr); return v; }

    // Operations
    sfpi_inline vUInt operator&(uint32_t b) const { return this->__vIntBase::operator&(vUInt(b)); }
    sfpi_inline vUInt operator&(const vUInt b) const { return this->__vIntBase::operator&(b); }
    sfpi_inline vUInt operator&=(const vUInt b) { this->__vIntBase::operator&=(b); return v; }
    sfpi_inline vUInt operator|(const vUInt b) const { return this->__vIntBase::operator|(b); }
    sfpi_inline vUInt operator|(const vUInt b) { return this->__vIntBase::operator|(b); }
    sfpi_inline vUInt operator|=(const vUInt b) { this->__vIntBase::operator|=(b); return v; }
    sfpi_inline vUInt operator^(const vUInt b) const { return this->__vIntBase::operator^(b); }
    sfpi_inline vUInt operator^(const vUInt b) { return this->__vIntBase::operator^(b); }
    sfpi_inline vUInt operator^=(const vUInt b) { this->__vIntBase::operator^=(b); return v; }
    sfpi_inline vUInt operator~() const { return this->__vIntBase::operator~<vUInt>(); }

    sfpi_inline vUInt operator+(int32_t val) const { return this->__vIntBase::add<vUInt>(val, 0); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vUInt operator+(const vType val) const { return this->__vIntBase::operator+<vUInt>(val); }
    sfpi_inline vUInt operator+(const __vConstIntBase val) const { return this->__vIntBase::operator+<vUInt>(val); }

    sfpi_inline vUInt operator-(int32_t val) const { return this->__vIntBase::sub<vUInt>(val, 0); }
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vUInt operator-(const vType val) const { return this->__vIntBase::operator-<vUInt>(val); }
    sfpi_inline vUInt operator-(const __vConstIntBase val) const { return this->__vIntBase::operator+<vUInt>(val); }

    sfpi_inline vUInt operator+=(int32_t val) { this->__vIntBase::add_eq<vUInt>(val, 0); return v; }
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vUInt operator+=(const vType val) { this->__vIntBase::operator+=<vUInt>(val); return v; }
    sfpi_inline vUInt operator+=(const __vConstIntBase val) { this->__vIntBase::operator+=<vUInt>(val); return v; }

    sfpi_inline vUInt operator-=(int32_t val) { this->__vIntBase::sub_eq<vUInt>(val, 0); return v; }
    template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>* = nullptr>
    sfpi_inline vUInt operator-=(const vType val) { this->__vIntBase::operator-=<vUInt>(val); return v; }
    sfpi_inline vUInt operator-=(const __vConstIntBase val) { this->__vIntBase::operator-=<vUInt>(val); return v; }

    sfpi_inline vUInt operator++(const int) { *this += 1; return *this; }
    sfpi_inline vUInt operator++() { vUInt tmp = *this; *this += 1; return tmp; }
    sfpi_inline vUInt operator--(const int) { *this -= 1; return *this; }
    sfpi_inline vUInt operator--() { vUInt tmp = *this; *this -= 1; return tmp; }

    sfpi_inline vUInt operator<<(uint32_t amt) const { return this->__vIntBase::operator<<<vUInt>(amt); }
    sfpi_inline vUInt operator>>(uint32_t amt) const;
    sfpi_inline vUInt operator<<=(uint32_t amt) { this->__vIntBase::operator<<=<vUInt>(amt); return v; }
    sfpi_inline vUInt operator>>=(uint32_t amt);

    // Conditionals
    sfpi_inline const __vCond operator==(int32_t val) const;
    sfpi_inline const __vCond operator!=(int32_t val) const;
    sfpi_inline const __vCond operator<(int32_t val) const;
    sfpi_inline const __vCond operator<=(int32_t val) const;
    sfpi_inline const __vCond operator>(int32_t val) const;
    sfpi_inline const __vCond operator>=(int32_t val) const;

    sfpi_inline const __vCond operator==(const __vIntBase src) const;
    sfpi_inline const __vCond operator!=(const __vIntBase src) const;
    sfpi_inline const __vCond operator<(const __vIntBase src) const;
    sfpi_inline const __vCond operator<=(const __vIntBase src) const;
    sfpi_inline const __vCond operator>(const __vIntBase src) const;
    sfpi_inline const __vCond operator>=(const __vIntBase src) const;
};

//////////////////////////////////////////////////////////////////////////////
class __vCond {
    friend class __vCCCtrl;
    friend class vInt;
    friend class vUInt;

 private:
    enum class vBoolOpType {
        vBoolOr = SFPXBOOL_MOD1_OR,
        vBoolAnd = SFPXBOOL_MOD1_AND,
        vBoolNot = SFPXBOOL_MOD1_NOT,
    };

    int result;

    sfpi_inline int get() const { return result; }

 public:
    enum __vCondOpType {
        __vCondLT = SFPXCMP_MOD1_CC_LT,
        __vCondNE = SFPXCMP_MOD1_CC_NE,
        __vCondGTE = SFPXCMP_MOD1_CC_GTE,
        __vCondEQ = SFPXCMP_MOD1_CC_EQ,
        __vCondLTE = SFPXCMP_MOD1_CC_LTE,
        __vCondGT = SFPXCMP_MOD1_CC_GT,
    };

    // Bool
    sfpi_inline __vCond(vBoolOpType t, const __vCond& a, const __vCond& b) { result = __builtin_rvtt_sfpxbool((int)t, a.get(), b.get()); }

    // Float
    sfpi_inline __vCond(const __vCondOpType t, const vFloat a, const float b)
    { result = __builtin_rvtt_sfpxfcmps(a.get(), __f32asui(b), t | SFPXSCMP_MOD1_FMT_FLOAT); }

    sfpi_inline __vCond(const __vCondOpType t, const vFloat a, const s2vFloat16 b)
    { result = __builtin_rvtt_sfpxfcmps(a.get(), b.get(), t | ((b.get_format() == SFPLOADI_MOD0_FLOATA) ? SFPXSCMP_MOD1_FMT_A : SFPXSCMP_MOD1_FMT_B)); }

    sfpi_inline __vCond(const __vCondOpType t, const vFloat a, const vFloat b)
    { result = __builtin_rvtt_sfpxfcmpv(a.get(), b.get(), t); }

    // Int
    sfpi_inline __vCond(const __vCondOpType t, const __vIntBase a, int32_t b, uint32_t mod)
    { result = __builtin_rvtt_sfpxicmps(a.get(), b, mod | t); }

    sfpi_inline __vCond(const __vCondOpType t, const __vIntBase a, const __vIntBase b, uint32_t mod)
    { result = __builtin_rvtt_sfpxicmpv(a.get(), b.get(), mod | t); }

    // Create from an integer context
    sfpi_inline __vCond(const vInt a) { result = __builtin_rvtt_sfpxicmps(a.get(), 0, __vCondNE); }

    // Create boolean operations from conditional operations
    sfpi_inline const __vCond operator&&(const __vCond& b) const { return __vCond(vBoolOpType::vBoolAnd, *this, b); }
    sfpi_inline const __vCond operator||(const __vCond& b) const { return __vCond(vBoolOpType::vBoolOr, *this, b); }
    sfpi_inline const __vCond operator!() const { return __vCond(vBoolOpType::vBoolNot, *this, *this); }
};

//////////////////////////////////////////////////////////////////////////////
class __vCCCtrl {
protected:
    int top;
    int push_count;

public:
    sfpi_inline __vCCCtrl();
    sfpi_inline ~__vCCCtrl();

    sfpi_inline void cc_if(const __vCond& op) const;
    sfpi_inline void cc_if(const __vIntBase& b) const;
    sfpi_inline void cc_else() const;
    sfpi_inline void cc_elseif(const __vCond& cond) const;
    sfpi_inline void cc_elseif(const __vIntBase& b) const;

    sfpi_inline void mark_top();
    sfpi_inline void push();
    sfpi_inline void pop();

    static sfpi_inline void enable_cc();
};

//////////////////////////////////////////////////////////////////////////////
constexpr __vConstFloat vConst0(CREG_IDX_0);
constexpr __vConstFloat vConst1(CREG_IDX_1);
constexpr __vConstFloat vConstNeg1(CREG_IDX_NEG_1);

//////////////////////////////////////////////////////////////////////////////
namespace sfpi_int {

sfpi_inline vFloat fp_add(const vFloat a, const vFloat b)
{
    return __builtin_rvtt_sfpadd(a.get(), b.get(), 0);
}

sfpi_inline vFloat fp_mul(const vFloat a, const vFloat b)
{
    return __builtin_rvtt_sfpmul(a.get(), b.get(), 0);
}

sfpi_inline vFloat fp_sub(const vFloat a, const vFloat b)
{
    __rvtt_vec_t neg1 = __builtin_rvtt_sfpassignlreg(vConstNeg1.get());
    return __builtin_rvtt_sfpmad(neg1, b.get(), a.get(), 0);
}

}

template<class TYPE, int N>
sfpi_inline TYPE __RegFile<TYPE, N>::operator[](const int x) const {
    return TYPE(__vRegBaseInitializer(x));
}

sfpi_inline vInt __vDReg::operator=(const int i) const
{
    vInt v(i);
    *this = v;
    return v;
}

sfpi_inline vUInt __vDReg::operator=(const unsigned int i) const
{
    vUInt v(i);
    *this = v;
    return v;
}

sfpi_inline vFloat __vDReg::operator+(const vFloat b) const {return sfpi_int::fp_add(vFloat(*this), b); }
sfpi_inline vFloat __vDReg::operator-(const vFloat b) const { return sfpi_int::fp_sub(vFloat(*this), b); }
sfpi_inline vFloat __vDReg::operator*(const vFloat b) const  { return sfpi_int::fp_mul(vFloat(*this), b); }
sfpi_inline __vCond __vDReg::operator==(const s2vFloat16 x) const {return __vCond(__vCond::__vCondEQ, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator!=(const s2vFloat16 x) const { return __vCond(__vCond::__vCondNE, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator<(const s2vFloat16 x) const { return __vCond(__vCond::__vCondLT, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator<=(const s2vFloat16 x) const { return __vCond(__vCond::__vCondLTE, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator>(const s2vFloat16 x) const { return __vCond(__vCond::__vCondGT, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator>=(const s2vFloat16 x) const { return __vCond(__vCond::__vCondGTE, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator==(const vFloat x) const {return __vCond(__vCond::__vCondEQ, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator!=(const vFloat x) const { return __vCond(__vCond::__vCondNE, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator<(const vFloat x) const { return __vCond(__vCond::__vCondLT, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator<=(const vFloat x) const { return __vCond(__vCond::__vCondLTE, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator>(const vFloat x) const { return __vCond(__vCond::__vCondGT, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator>=(const vFloat x) const { return __vCond(__vCond::__vCondGTE, vFloat(*this), x); }

sfpi_inline vFloat __vDReg::operator-() const
{
    vFloat tmp = *this;
    return __builtin_rvtt_sfpmov(tmp.get(), SFPMOV_MOD1_COMPSIGN);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void __vBase::assign(const __rvtt_vec_t in)
{
    v = (initialized) ? __builtin_rvtt_sfpassign_lv(v, in) : in;
    initialized = true;
}

sfpi_inline void __vBase::operator=(__vLReg lr)
{
    v = __builtin_rvtt_sfpassignlreg(lr.get());
    initialized = true;
}

sfpi_inline __rvtt_vec_t __vLReg::operator=(__vBase& v) const
{
    __builtin_rvtt_sfppreservelreg(v.get(), reg);
    return v.get();
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat vFloat::operator+(const vFloat b) const { return sfpi_int::fp_add(*this, b); }
sfpi_inline vFloat vFloat::operator-(const vFloat b) const { return sfpi_int::fp_sub(*this, b); }
sfpi_inline vFloat vFloat::operator-(const float b) const { return sfpi_int::fp_add(*this, vFloat(-b)); }
sfpi_inline vFloat vFloat::operator-(const s2vFloat16 b) const { return sfpi_int::fp_add(*this, b.negate()); }
sfpi_inline vFloat vFloat::operator*(const vFloat b) const { return sfpi_int::fp_mul(*this, b); }
sfpi_inline __vCond vFloat::operator==(const s2vFloat16 x) const { return __vCond(__vCond::__vCondEQ, *this, x); }
sfpi_inline __vCond vFloat::operator!=(const s2vFloat16 x) const { return __vCond(__vCond::__vCondNE, *this, x); }
sfpi_inline __vCond vFloat::operator<(const s2vFloat16 x) const { return __vCond(__vCond::__vCondLT, *this, x); }
sfpi_inline __vCond vFloat::operator<=(const s2vFloat16 x) const { return __vCond(__vCond::__vCondLTE, *this, x); }
sfpi_inline __vCond vFloat::operator>(const s2vFloat16 x) const { return __vCond(__vCond::__vCondGT, *this, x); }
sfpi_inline __vCond vFloat::operator>=(const s2vFloat16 x) const { return __vCond(__vCond::__vCondGTE, *this, x); }
sfpi_inline __vCond vFloat::operator==(const vFloat x) const { return __vCond(__vCond::__vCondEQ, *this, x); }
sfpi_inline __vCond vFloat::operator!=(const vFloat x) const { return __vCond(__vCond::__vCondNE, *this, x); }
sfpi_inline __vCond vFloat::operator<(const vFloat x) const { return __vCond(__vCond::__vCondLT, *this, x); }
sfpi_inline __vCond vFloat::operator<=(const vFloat x) const { return __vCond(__vCond::__vCondLTE, *this, x); }
sfpi_inline __vCond vFloat::operator>(const vFloat x) const { return __vCond(__vCond::__vCondGT, *this, x); }
sfpi_inline __vCond vFloat::operator>=(const vFloat x) const { return __vCond(__vCond::__vCondGTE, *this, x); }

sfpi_inline vFloat vFloat::operator*=(const vFloat m)
{
    assign(__builtin_rvtt_sfpmul(v, m.get(), SFPMAD_MOD1_OFFSET_NONE));
    return v;
}

sfpi_inline vFloat vFloat::operator+=(const vFloat a)
{
    assign(__builtin_rvtt_sfpadd(v, a.get(), SFPMAD_MOD1_OFFSET_NONE));
    return v;
}

sfpi_inline vFloat vFloat::operator-() const
{
    return __builtin_rvtt_sfpmov(v, SFPMOV_MOD1_COMPSIGN);
    return v;
}

sfpi_inline void vFloat::loadf16(const s2vFloat16 val)
{
    assign(__builtin_rvtt_sfpxloadi(val.get_format(), val.get()));
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void __vIntBase::loadss(int16_t val)
{
    assign(__builtin_rvtt_sfpxloadi(SFPLOADI_MOD0_SHORT, val));
}

sfpi_inline void __vIntBase::loadus(uint16_t val)
{
    assign(__builtin_rvtt_sfpxloadi(SFPLOADI_MOD0_USHORT, val));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator&(const vType b) const
{
    return __builtin_rvtt_sfpand(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline void __vIntBase::operator&=(const vType b)
{
    v = __builtin_rvtt_sfpand(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator|(const vType b) const
{
    return __builtin_rvtt_sfpor(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator|=(const vType b)
{
    v = __builtin_rvtt_sfpor(v, b.get());
    return v;
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator~() const
{
    return __builtin_rvtt_sfpnot(v);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator<<(uint32_t amt) const
{
    return __builtin_rvtt_sfpshft_i(v, amt);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator<<=(uint32_t amt)
{
    assign((static_cast<vType>(*this) << amt).get());
    return v;
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::add(int32_t val, unsigned int mod_base) const
{
    return __builtin_rvtt_sfpxiadd_i(v, val, mod_base);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator+(const __vIntBase val) const
{
    return __builtin_rvtt_sfpxiadd_v(val.get(), v, 0);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator+(const __vConstIntBase val) const
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlreg(val.get());
    return __builtin_rvtt_sfpxiadd_v(c, v, 0);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::sub(int32_t val, unsigned int mod_base) const
{
    return __builtin_rvtt_sfpxiadd_i(v, val, mod_base | SFPXIADD_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator-(const __vIntBase val) const
{
    return __builtin_rvtt_sfpxiadd_v(val.get(), v, SFPXIADD_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator-(const __vConstIntBase val) const
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlreg(val.get());
    return __builtin_rvtt_sfpxiadd_v(c, v, SFPXIADD_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline void __vIntBase::add_eq(int32_t val, unsigned int mod_base)
{
    assign(__builtin_rvtt_sfpxiadd_i(v, val, mod_base));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator+=(const __vIntBase val)
{
    assign(__builtin_rvtt_sfpxiadd_v(v, val.get(), 0));
    return v;
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator+=(const __vConstIntBase val)
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlreg(val.get());
    assign(__builtin_rvtt_sfpxiadd_v(c, v, 0));
    return v;
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline void __vIntBase::sub_eq(int32_t val, unsigned int mod_base)
{
    assign(__builtin_rvtt_sfpxiadd_i(v, val, mod_base | SFPXIADD_MOD1_IS_SUB));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator-=(const __vIntBase val)
{
    assign(__builtin_rvtt_sfpxiadd_v(val.get(), v, SFPXIADD_MOD1_IS_SUB));
    return v;
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator-=(const __vConstIntBase val)
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlreg(val.get());
    assign(__builtin_rvtt_sfpxiadd_v(c, v, SFPXIADD_MOD1_IS_SUB));
    return v;
}

sfpi_inline vUInt vUInt::operator>>(uint32_t amt) const
{
    return __builtin_rvtt_sfpshft_i(v, -amt);
}

sfpi_inline vUInt vUInt::operator>>=(uint32_t amt)
{
    assign((*this >> amt).get());
    return v;
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat operator+(const float a, const vFloat b) { return b + a; }
sfpi_inline vFloat operator*(const float a, const vFloat b) { return b * a; }
sfpi_inline vFloat operator-(const float a, const vFloat b) { return vFloat(a) - b; }
sfpi_inline __vCond operator==(const float a, const vFloat b) { return b == a; }
sfpi_inline __vCond operator!=(const float a, const vFloat b) { return b != a; }
sfpi_inline __vCond operator<(const float a, const vFloat b) { return b > a; }
sfpi_inline __vCond operator<=(const float a, const vFloat b) { return b >= a; }
sfpi_inline __vCond operator>(const float a, const vFloat b) { return b < a; }
sfpi_inline __vCond operator>=(const float a, const vFloat b) { return b <= a; }
sfpi_inline __vCond operator==(const s2vFloat16 a, const vFloat b) { return b == a; }
sfpi_inline __vCond operator!=(const s2vFloat16 a, const vFloat b) { return b != a; }
sfpi_inline __vCond operator<(const s2vFloat16 a, const vFloat b) { return b > a; }
sfpi_inline __vCond operator<=(const s2vFloat16 a, const vFloat b) { return b >= a; }
sfpi_inline __vCond operator>(const s2vFloat16 a, const vFloat b) { return b < a; }
sfpi_inline __vCond operator>=(const s2vFloat16 a, const vFloat b) { return b <= a; }

sfpi_inline vInt operator+(const int32_t a, const vInt b) { return b + a; }
sfpi_inline vInt operator-(const int32_t a, const vInt b) { return vInt(a) - b; }
sfpi_inline vInt operator&(const int32_t a, const vInt b) { return b & a; }
sfpi_inline vInt operator|(const int32_t a, const vInt b) { return b | a; }
sfpi_inline vInt operator^(const int32_t a, const vInt b) { return b ^ a; }
sfpi_inline __vCond operator==(const int32_t a, const vInt b) { return b == a; }
sfpi_inline __vCond operator!=(const int32_t a, const vInt b) { return b != a; }
sfpi_inline __vCond operator<(const int32_t a, const vInt b) { return b > a; }
sfpi_inline __vCond operator<=(const int32_t a, const vInt b) { return b >= a; }
sfpi_inline __vCond operator>(const int32_t a, const vInt b) { return b < a; }
sfpi_inline __vCond operator>=(const int32_t a, const vInt b) { return b <= a; }

sfpi_inline vUInt operator+(const int32_t a, const vUInt b) { return b + a; }
sfpi_inline vUInt operator-(const int32_t a, const vUInt b) { return vUInt(a) - b; }
sfpi_inline vUInt operator&(const int32_t a, const vUInt b) { return b & a; }
sfpi_inline vUInt operator|(const int32_t a, const vUInt b) { return b | a; }
sfpi_inline vUInt operator^(const int32_t a, const vUInt b) { return b ^ a; }
sfpi_inline __vCond operator==(const int32_t a, const vUInt b) { return b == a; }
sfpi_inline __vCond operator!=(const int32_t a, const vUInt b) { return b != a; }
sfpi_inline __vCond operator<(const int32_t a, const vUInt b) { return b > a; }
sfpi_inline __vCond operator<=(const int32_t a, const vUInt b) { return b >= a; }
sfpi_inline __vCond operator>(const int32_t a, const vUInt b) { return b < a; }
sfpi_inline __vCond operator>=(const int32_t a, const vUInt b) { return b <= a; }

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat __vConstFloat::operator+(const vFloat b) const { return vFloat(*this) + b; }
sfpi_inline vFloat __vConstFloat::operator-(const vFloat b) const { return vFloat(*this) - b; }
sfpi_inline vFloat __vConstFloat::operator*(const vFloat b) const { return vFloat(*this) * b; }
sfpi_inline __vCond __vConstFloat::operator==(const vFloat x) const { return __vCond(__vCond::__vCondEQ, *this, x); }
sfpi_inline __vCond __vConstFloat::operator!=(const vFloat x) const { return __vCond(__vCond::__vCondNE, *this, x); }
sfpi_inline __vCond __vConstFloat::operator<(const vFloat x) const { return __vCond(__vCond::__vCondLT, *this, x); }
sfpi_inline __vCond __vConstFloat::operator<=(const vFloat x) const { return __vCond(__vCond::__vCondLTE, vFloat(*this), x); }
sfpi_inline __vCond __vConstFloat::operator>(const vFloat x) const { return __vCond(__vCond::__vCondGT, vFloat(*this), x); }
sfpi_inline __vCond __vConstFloat::operator>=(const vFloat x) const { return __vCond(__vCond::__vCondGTE, *this, x); }

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vConstIntBase::operator+(const vType b) const { return vType(*this) + b; }
sfpi_inline vInt __vConstIntBase::operator+(int32_t b) const { return vInt(*this) + b; }

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vConstIntBase::operator-(const vType b) const { return vType(*this) - b; }
sfpi_inline vInt __vConstIntBase::operator-(int32_t b) const { return vInt(*this) - b; }

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vConstIntBase::operator&(const vType b) const { return vType(*this) & b; }
sfpi_inline vInt __vConstIntBase::operator&(int32_t b) const { return vInt(*this) & vInt(b); }

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vConstIntBase::operator|(const vType b) const { return vType(*this) | b; }
sfpi_inline vInt __vConstIntBase::operator|(int32_t b) const { return vInt(*this) | vInt(b); }

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vConstIntBase::operator^(const vType b) const { return vType(*this) ^ b; }
sfpi_inline vInt __vConstIntBase::operator^(int32_t b) const { return vInt(*this) ^ vInt(b); }

sfpi_inline __vCond __vConstIntBase::operator==(const vInt x) const { return __vCond(__vCond::__vCondEQ, __vIntBase(*this), x, 0); }
sfpi_inline __vCond __vConstIntBase::operator!=(const vInt x) const { return __vCond(__vCond::__vCondNE, __vIntBase(*this), x, 0); }
sfpi_inline __vCond __vConstIntBase::operator<(const vInt x) const { return __vCond(__vCond::__vCondLT, __vIntBase(*this), x, 0); }
sfpi_inline __vCond __vConstIntBase::operator<=(const vInt x) const { return __vCond(__vCond::__vCondLTE, __vIntBase(*this), x, 0); }
sfpi_inline __vCond __vConstIntBase::operator>(const vInt x) const { return __vCond(__vCond::__vCondGT, __vIntBase(*this), x, 0); }
sfpi_inline __vCond __vConstIntBase::operator>=(const vInt x) const { return __vCond(__vCond::__vCondGTE, __vIntBase(*this), x, 0); }

sfpi_inline __vIntBase __vConstIntBase::operator<<(uint32_t amt) const
{
    __rvtt_vec_t v = __builtin_rvtt_sfpassignlreg(reg);
    return __builtin_rvtt_sfpshft_i(v, amt);
}

sfpi_inline vUInt __vConstIntBase::operator>>(uint32_t amt) const
{
    __rvtt_vec_t v = __builtin_rvtt_sfpassignlreg(reg);
    return __builtin_rvtt_sfpshft_i(v, -amt);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat::vFloat(const __vConstFloat creg)
{
    v = __builtin_rvtt_sfpassignlreg(creg.get());
    initialized = true;
}

sfpi_inline __vIntBase::__vIntBase(const __vConstIntBase creg)
{
    v = __builtin_rvtt_sfpassignlreg(creg.get());
    initialized = true;
}

sfpi_inline vInt::vInt(const __vCond vc)
{
    v = __builtin_rvtt_sfpxcondi(vc.get());
    initialized = true;
}

sfpi_inline const __vCond vInt::operator==(int32_t val) const { return __vCond(__vCond::__vCondEQ, *this, val, SFPXIADD_MOD1_SIGNED); }
sfpi_inline const __vCond vInt::operator!=(int32_t val) const { return __vCond(__vCond::__vCondNE, *this, val, SFPXIADD_MOD1_SIGNED); }
sfpi_inline const __vCond vInt::operator<(int32_t val) const { return __vCond(__vCond::__vCondLT, *this, val, SFPXIADD_MOD1_SIGNED); }
sfpi_inline const __vCond vInt::operator<=(int32_t val) const { return  __vCond(__vCond::__vCondLTE, *this, val, SFPXIADD_MOD1_SIGNED); }
sfpi_inline const __vCond vInt::operator>(int32_t val) const { return  __vCond(__vCond::__vCondGT, *this, val, SFPXIADD_MOD1_SIGNED); }
sfpi_inline const __vCond vInt::operator>=(int32_t val) const { return __vCond(__vCond::__vCondGTE, *this, val, SFPXIADD_MOD1_SIGNED); }

sfpi_inline const __vCond vInt::operator==(const __vIntBase src) const { return __vCond(__vCond::__vCondEQ, src, *this, 0); }
sfpi_inline const __vCond vInt::operator!=(const __vIntBase src) const { return __vCond(__vCond::__vCondNE, src, *this, 0); }
sfpi_inline const __vCond vInt::operator<(const __vIntBase src) const { return __vCond(__vCond::__vCondLT, src, *this, 0); }
sfpi_inline const __vCond vInt::operator<=(const __vIntBase src) const { return __vCond(__vCond::__vCondLTE, src, *this, 0); }
sfpi_inline const __vCond vInt::operator>(const __vIntBase src) const { return __vCond(__vCond::__vCondGT, src, *this, 0); }
sfpi_inline const __vCond vInt::operator>=(const __vIntBase src) const { return __vCond(__vCond::__vCondGTE, src, *this, 0); }

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vUInt::vUInt(const __vCond vc)
{
    v = __builtin_rvtt_sfpxcondi(vc.get());
    initialized = true;
}

sfpi_inline const __vCond vUInt::operator==(int32_t val) const { return __vCond(__vCond::__vCondEQ, *this, val, 0); }
sfpi_inline const __vCond vUInt::operator!=(int32_t val) const { return __vCond(__vCond::__vCondNE, *this, val, 0); }
sfpi_inline const __vCond vUInt::operator<(int32_t val) const { return __vCond(__vCond::__vCondLT, *this, val, 0); }
sfpi_inline const __vCond vUInt::operator<=(int32_t val) const { return  __vCond(__vCond::__vCondLTE, *this, val, 0); }
sfpi_inline const __vCond vUInt::operator>(int32_t val) const { return  __vCond(__vCond::__vCondGT, *this, val, 0); }
sfpi_inline const __vCond vUInt::operator>=(int32_t val) const { return __vCond(__vCond::__vCondGTE, *this, val, 0); }

sfpi_inline const __vCond vUInt::operator==(const __vIntBase src) const { return __vCond(__vCond::__vCondEQ, src, *this, 0); }
sfpi_inline const __vCond vUInt::operator!=(const __vIntBase src) const { return __vCond(__vCond::__vCondNE, src, *this, 0); }
sfpi_inline const __vCond vUInt::operator<(const __vIntBase src) const { return __vCond(__vCond::__vCondLT, src, *this, 0); }
sfpi_inline const __vCond vUInt::operator<=(const __vIntBase src) const { return __vCond(__vCond::__vCondLTE, src, *this, 0); }
sfpi_inline const __vCond vUInt::operator>(const __vIntBase src) const { return __vCond(__vCond::__vCondGT, src, *this, 0); }
sfpi_inline const __vCond vUInt::operator>=(const __vIntBase src) const { return __vCond(__vCond::__vCondGTE, src, *this, 0); }

//////////////////////////////////////////////////////////////////////////////
sfpi_inline __vCCCtrl::__vCCCtrl() : push_count(0)
{
    push();
}

sfpi_inline void __vCCCtrl::cc_if(const __vCond& op) const
{
    __builtin_rvtt_sfpxcondb(op.get(), top);
}

sfpi_inline void __vCCCtrl::cc_if(const __vIntBase& v) const
{
    __builtin_rvtt_sfpxcondb(__vCond(__vCond::__vCondNE, v, 0, 0).get(), top);
}

sfpi_inline void __vCCCtrl::cc_elseif(const __vCond& op) const
{
    cc_if(op);
}

sfpi_inline void __vCCCtrl::cc_elseif(const __vIntBase& v) const
{
    cc_if(v);
}

sfpi_inline void __vCCCtrl::cc_else() const
{
    __builtin_rvtt_sfpcompc();
}

sfpi_inline __vCCCtrl::~__vCCCtrl()
{
    while (push_count != 0) {
        pop();
    }
}

sfpi_inline void __vCCCtrl::mark_top()
{
    top = __builtin_rvtt_sfpxvif();
}

sfpi_inline void __vCCCtrl::push()
{
    push_count++;
    __builtin_rvtt_sfppushc();
}

sfpi_inline void __vCCCtrl::pop()
{
    push_count--;
    __builtin_rvtt_sfppopc();
}

sfpi_inline void __vCCCtrl::enable_cc()
{
    __builtin_rvtt_sfpencc(SFPENCC_IMM12_BOTH, SFPENCC_MOD1_EI_RI);
}

//////////////////////////////////////////////////////////////////////////////
constexpr __DestReg dst_reg;
constexpr __LReg l_reg;

} // namespace sfpi

//////////////////////////////////////////////////////////////////////////////
#define v_if(x)             \
{                           \
   __vCCCtrl __cc;          \
   __cc.mark_top();         \
   __cc.cc_if(x);

#define v_elseif(x)         \
    __cc.cc_else();         \
    __cc.push();            \
    __cc.mark_top();        \
    __cc.cc_elseif(x);

#define v_else              \
    __cc.cc_else();

#define v_endif             \
}

#define v_block             \
{                           \
    __vCCCtrl __cc;

#define v_and(x)            \
    __cc.mark_top();        \
    __cc.cc_if(x)

#define v_endblock          \
}

//////////////////////////////////////////////////////////////////////////////
#if defined(ARCH_GRAYSKULL)
#include <grayskull/sfpi_imp.h>
#include <grayskull/sfpi_lib.h>
#elif defined(ARCH_WORMHOLE)
#include <wormhole/sfpi_imp.h>
#include <wormhole/sfpi_lib.h>
#endif
