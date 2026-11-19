// { dg-options "-std=c++17 -ftt-consteval" }

#include <type_traits>

#if __cpp_constexpr < 202002L
#error "bad constexpr"
#endif
#if __cpp_consteval< 202211L
#error "bad consteval"
#endif

consteval int f () {
  return 0;
}

int frob (int);
constexpr int compute(int x) {
  if (std::is_constant_evaluated())
    return x * x;
  return frob (x);
}

constexpr int i = compute (3);
