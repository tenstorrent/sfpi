/* { dg-do compile } */
/* { dg-options "-mcpu=tt-bh -O2" } */
// Taken from gcc.target/riscv/zbb test

unsigned long long foo1(unsigned long long rs1, unsigned long long rs2)
{
return rs1 & ~rs2;
}

unsigned long long foo2(unsigned long long rs1, unsigned long long rs2)
{
return rs1 | ~rs2;
}

unsigned long long foo3(unsigned long long rs1, unsigned long long rs2)
{
return rs1 ^ ~rs2;
}

/* { dg-final { scan-assembler-times "\n\tandn	" 2 } } */
/* { dg-final { scan-assembler-times "\n\torn	" 2 } } */
/* { dg-final { scan-assembler-times "\n\txnor	" 2 } } */
