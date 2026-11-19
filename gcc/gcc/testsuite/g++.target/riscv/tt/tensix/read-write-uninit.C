// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }

using __rvtt_vec_t = ::__xtt_vector;

struct X {
  int a;
  __rvtt_vec_t b;
};

void use (X *);
void foo (__rvtt_vec_t *ptr) {
  __rvtt_vec_t a; // uninit
  __builtin_rvtt_sfpstore (nullptr, a, 0, 0, 0, 0, 0); // { dg-error "is not initialized" }

  *ptr  = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);  // { dg-error "cannot write" }

  use (nullptr);
  
  __builtin_rvtt_sfpstore (nullptr, *ptr, 0, 0, 0, 0, 0);  // { dg-error "cannot read" }
  X val;
  val.b = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);  // { dg-error "cannot write" }
  use (&val);
  __builtin_rvtt_sfpstore (nullptr, val.b, 0, 0, 0, 0, 0);  // { dg-error "cannot read" }
}
