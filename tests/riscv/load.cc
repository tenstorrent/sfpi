// This file contains tests for load latency and riscv L1 load intrinsic

#include <cstdio>

#define l1_load(type) __builtin_rvtt_l1_load_##type

static const int *gptr = (int *)0xffff0000;

typedef union {
    long long int li;
    struct {
        unsigned int upper;
        unsigned int lower;
    };
} lint;

typedef union {
    unsigned long long int uli;
    struct {
        unsigned int upper;
        unsigned int lower;
    };
} ulint;

signed char array_qi[5] = {0, -1, -2, -3, -4};
short array_hi[5] = {0, -1, -2, -3, -4};
int array_si[5] = {0, -1, -2, -3, -4};
lint array_di[5] = {0, -1, -2, -3, -4};
unsigned char array_uqi[5] = {0, 1, 2, 3, 4};
unsigned short array_uhi[5] = {0, 1, 2, 3, 4};
unsigned int array_usi[5] = {0, 1, 2, 3, 4};
ulint array_udi[5] = {0, 1, 2, 3, 4};
#if 0
// Compiler can't break this up for scheduling
int latency_chain_dependent(int *p, int a, int b, int c, int d, int e)
{
    return *p + a + b + c + d + e;
}

// This gets scheduled as the + dependency chains can be interleaved
int latency_chain_split(int *p, int a, int b, int c, int d, int e)
{
    int x = *p + a;
    int y = b + c;
    int z = d + e;

    return x + y + z;
}

int latency_chain_dependent_intrinsic(int *p, int a, int b, int c, int d, int e)
{
    return l1_load(si)(*p) + a + b + c + d + e;
}

int latency_chain_split_intrinsic(int *p, int a, int b, int c, int d, int e)
{
    int x = l1_load(si)(*p) + a;
    int y = b + c;
    int z = d + e;

    return x + y + z;
}

int addr_calc1(int *ptr)
{
    return *(ptr + 5);
}

int addr_calc2()
{
    return array_si[3];
}

int addr_calc3()
{
    return *(gptr + 5);
}

int addr_calc_stack()
{
    volatile int v = 3;
    volatile int *vp = &v;
    return *vp;
}

int addr_calc_intrin1(int *ptr)
{
    return l1_load(si)(*(ptr + 5));
}

int addr_calc_intrin2()
{
    return l1_load(si)(array_si[3]);
}

int addr_calc_intrin3()
{
    return l1_load(si)(*(gptr + 5));
}

int addr_calc_intrin_stack()
{
    int v = 0;
    return l1_load(si)(v);
}

void * intrin_ptr(void *ptr)
{
    return l1_load(ptr)(ptr);
}

signed char intrin_qi()
{
    return l1_load(qi)(array_qi[2]);
}

int intrin_qi2()
{
    return l1_load(qi)(array_qi[2]);
}

short intrin_hi()
{
    return l1_load(hi)(array_hi[2]);
}

int intrin_hi2()
{
    return l1_load(hi)(array_hi[2]);
}

int intrin_si()
{
    return l1_load(si)(array_si[2]);
}

unsigned char intrin_uqi()
{
    return l1_load(uqi)(array_uqi[2]);
}

unsigned int intrin_uqi2()
{
    return l1_load(uqi)(array_uqi[2]);
}

unsigned short intrin_uhi()
{
    return l1_load(uhi)(array_uhi[2]);
}

unsigned int intrin_uhi2()
{
    return l1_load(uhi)(array_uhi[2]);
}

unsigned int intrin_usi()
{
    return l1_load(usi)(array_usi[2]);
}

long long int intrin_di()
{
    lint out;
    out.upper = l1_load(si)(array_di[2].upper);
    out.lower = l1_load(si)(array_di[2].lower);
    return out.li;
}

long long int di()
{
    return array_di[2].li;
}

unsigned long long int intrin_udi()
{
    ulint out;
    out.upper = l1_load(si)(array_udi[2].upper);
    out.lower = l1_load(si)(array_udi[2].lower);
    return out.uli;
}

unsigned long long int udi()
{
    return array_udi[2].uli;
}
int intrin_multiple_cast()
{
    return (int)(short)(int)l1_load(qi)(array_qi[2]);
}

unsigned short intrin_downcast1()
{
    return l1_load(si)(array_si[2]);
}

unsigned char intrin_downcast2()
{
    return l1_load(si)(array_si[2]);
}

int multiple_uses()
{
    unsigned short us = l1_load(hi)(array_hi[2]);
    signed char sc = us;
    signed int si = us;

    return sc + si;
}

int multiple_uses_base()
{
    unsigned short us = array_hi[2];
    signed char sc = us;
    signed int si = us;

    return sc + si;
}

int intervening_code(int a)
{
    unsigned short us = l1_load(hi)(array_hi[2]);

    int b = a << 4;
    
    return b + (int)us;
}

// Baseline w/o an intrinsic.  Default of unmodified char is unsigned on RISCV
signed char qi()
{
    return array_qi[2];
}

short hi()
{
    return array_hi[2];
}

int si()
{
    return array_si[2];
}

unsigned char uqi()
{
    return array_uqi[2];
}

unsigned short uhi()
{
    return array_uhi[2];
}

unsigned int usi()
{
    return array_usi[2];
}

// Test register loads.  There's only a USI version, so whew
unsigned int reg_read()
{
    return __builtin_rvtt_reg_read(array_usi[2]);
}

// People shouldn't be casting, so minimal testing here
unsigned short reg_read_downcast()
{
    return __builtin_rvtt_reg_read(array_usi[2]);
}
#endif

// Can't reproduce issue found in firmware where a loaded value was shifted
// and masked twice to two different regs, fix blind
void dual_use_bug(unsigned char *in, volatile unsigned int *out1, volatile unsigned char *out2)
{
    unsigned int x = l1_load(uqi)(in[0]);
    out1[0] = x;
    out2[0] = x;
}

int main(int argc, char *argv[])
{
    
}
