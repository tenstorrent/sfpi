// { dg-options "-std=c++17 -ftt-nttp" }

#if __cpp_nontype_template_args < 201911
#error "bad nttp"
#endif
#if __cpp_nontype_template_parameter_class < 201806
#error "bad nttp"
#endif

struct X {};

template<X v> void frob () {}
