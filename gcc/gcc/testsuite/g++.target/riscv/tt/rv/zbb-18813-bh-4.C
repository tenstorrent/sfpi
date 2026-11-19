/* { dg-do compile } */
/* { dg-options "-mcpu=tt-bh -O2" } */
// Taken from gcc.target/riscv/zbb test

long
foo1 (long i, long j)
{
  return i < j ? i : j;
}

long
foo2 (long i, long j)
{
  return i > j ? i : j;
}

unsigned long
foo3 (unsigned long i, unsigned long j)
{
  return i < j ? i : j;
}

unsigned long
foo4 (unsigned long i, unsigned long j)
{
  return i > j ? i : j;
}

/* { dg-final { scan-assembler "\n\tmin	" } } */
/* { dg-final { scan-assembler "\n\tmax	" } } */
/* { dg-final { scan-assembler "\n\tminu	" } } */
/* { dg-final { scan-assembler "\n\tmaxu	" } } */
