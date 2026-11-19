/* { dg-do compile } */
/* { dg-options "-mcpu=tt-bh -O2" } */
// Taken from gcc.target/riscv/zbb test

unsigned int foo1(unsigned int rs1, unsigned int rs2)
{
    unsigned int shamt = rs2 & (32 - 1);
    return (rs1 << shamt) | (rs1 >> ((32 - shamt) & (32 - 1)));
}
unsigned int foo2(unsigned int rs1, unsigned int rs2)
{
    unsigned int shamt = rs2 & (32 - 1);
    return (rs1 >> shamt) | (rs1 << ((32 - shamt) & (32 - 1)));
}

/* { dg-final { scan-assembler "\n\trol	" } } */
/* { dg-final { scan-assembler "\n\tror	" } } */
