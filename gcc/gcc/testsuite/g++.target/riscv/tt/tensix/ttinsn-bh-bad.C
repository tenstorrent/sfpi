// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }

void baz (unsigned v) {
  __builtin_rvtt_ttinsn (nullptr, v, 0, 0, 0); // { dg-error "not known at compile" }
}
