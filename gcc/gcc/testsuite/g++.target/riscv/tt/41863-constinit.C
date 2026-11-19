// { dg-options "-std=c++17 -ftt-constinit" }

#if __cpp_constinit < 201907L
#error "bad constinit"
#endif

constinit int f = 5;
