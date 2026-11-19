/* { dg-do compile } */
/* { dg-options "-mcpu=tt-bh -O2" } */
// Taken from gcc.target/riscv/zbb test

int
clz (int i)
{
  return __builtin_clz (i);
}

int
ctz (int i)
{
  return __builtin_ctz (i);
}

int
popcount (int i)
{
  return __builtin_popcount (i);
}


/* { dg-final { scan-assembler-times "\n\tclz	" 1 } } */
/* { dg-final { scan-assembler-times "\n\tctz	" 1 } } */
/* { dg-final { scan-assembler-times "\n\tcpop	" 1 } } */
