/* { dg-do compile } */
/* { dg-options "-mcpu=tt-bh -O2" } */
// Taken from gcc.target/riscv/zbb test

unsigned int foo1(unsigned int rs1, unsigned int rs2)
{
return rs1 & ~rs2;
}

unsigned int foo2(unsigned  int rs1, unsigned  int rs2)
{
return rs1 | ~rs2;
}

unsigned int foo3(unsigned int rs1, unsigned int rs2)
{
return rs1 ^ ~rs2;
}

/* { dg-final { scan-assembler "\n\tandn	" } } */
/* { dg-final { scan-assembler "\n\torn	" } } */
/* { dg-final { scan-assembler "\n\txnor	" } } */
