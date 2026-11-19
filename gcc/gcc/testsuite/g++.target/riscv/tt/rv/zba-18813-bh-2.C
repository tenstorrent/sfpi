/* { dg-do compile } */
/* { dg-options "-mcpu=tt-bh -O2" } */
// Taken from gcc.target/riscv/zba test

long test_1(long a, long b)
{
  return a + (b << 1);
}
long test_2(long a, long b)
{
  return a + (b << 2);
}
long test_3(long a, long b)
{
  return a + (b << 3);
}

/* { dg-final { scan-assembler "\n\tsh1add	" } } */
/* { dg-final { scan-assembler "\n\tsh2add	" } } */
/* { dg-final { scan-assembler "\n\tsh3add	" } } */
