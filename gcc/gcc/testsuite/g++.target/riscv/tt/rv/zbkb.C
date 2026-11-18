// { dg-do assemble }
// { dg-additional-options "-mcpu=tt-bh -O2" }

unsigned brev (unsigned a) {
  return __builtin_riscv_brev8_32 (a);
}

unsigned pack (unsigned lo, unsigned hi) {
  return __builtin_riscv_pack (hi, lo);
}
