// { dg-options "-mcpu=tt-qsr32-tensix -O2 -fno-exceptions -fno-rtti" }

void record () {
  __builtin_rvtt_ttreplay (nullptr, 20, 4, 0, 0, 1, 1, 0, 0); // { dg-error "Quasar replay" }
}
