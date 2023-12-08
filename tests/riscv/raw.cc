/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define l1p __attribute__((rvtt_l1_ptr))
#define regp __attribute__((rvtt_reg_ptr))

// This file contains tests for the read after write hazard bug workaround in GS/WH
__attribute__ ((noinline))
void basic_qi(volatile unsigned char *ptr)
{
    unsigned char x = 5;
    *(ptr + 4) = x;
}

__attribute__ ((noinline))
void basic_hi(volatile unsigned short int *ptr)
{
    unsigned short int x = 5;
    *(ptr + 4) = x;
}

__attribute__ ((noinline))
void basic_si(volatile unsigned int *ptr)
{
    unsigned int x = 5;
    *(ptr + 4) = x;
}

__attribute__ ((noinline))
void basic_l1_qi(volatile unsigned char l1p *ptr)
{
    unsigned char x = 5;
    *ptr = x;
}

__attribute__ ((noinline))
void basic_l1_hi(volatile unsigned short int l1p *ptr)
{
    unsigned short int x = 5;
    *ptr = x;
}

__attribute__ ((noinline))
void basic_l1_si(volatile unsigned int l1p *ptr)
{
    unsigned int x = 5;
    *ptr = x;
}

__attribute__ ((noinline))
void basic_reg_qi(volatile unsigned char regp *ptr)
{
    unsigned char x = 5;
    *ptr = x;
}

__attribute__ ((noinline))
void basic_reg_hi(volatile unsigned short int regp *ptr)
{
    unsigned short int x = 5;
    *ptr = x;
}

__attribute__ ((noinline))
void basic_reg_si(volatile unsigned int regp *ptr)
{
    unsigned int x = 5;
    *ptr = x;
}

__attribute__ ((noinline))
void multiple_stores(volatile unsigned short *sptr, volatile unsigned int *iptr)
{
    unsigned int x = 5;
    *sptr = x;
    *(sptr + 1) = x;
    *(iptr + 0) = x;
    *(sptr + 2) = x;
    *(sptr + 3) = x;
    *(iptr + 2) = x;
    *(sptr + 4) = x;
}

__attribute__ ((noinline))
void multiple_stores_with_loads(volatile unsigned short *sptr, volatile unsigned int *iptr)
{
    unsigned int x = 5;
    *sptr = x;
    *(sptr + 1) = x;
    *(iptr + 0) = x;
    x = *iptr;
    *(sptr + 2) = x;
    *(sptr + 3) = x;
    x = *(sptr + 1);
    *(iptr + 2) = x;
    *(sptr + 4) = x;
}

// uses local memory and so war uses r0
__attribute__ ((noinline))
void change_src_reg_r0(volatile unsigned short *sptr)
{
    unsigned int x = 5;

    *sptr = x;
    x = 6;
    *sptr = x;
}

// uses l1 memory and so war whatever register was used for the load on GS
__attribute__ ((noinline))
void change_src_reg_rn(volatile unsigned short l1p *sptr)
{
    unsigned int x = 5;

    *sptr = x;
    x = 6;
    *sptr = x;
}

__attribute__ ((noinline))
void change_dst_reg_rn(volatile unsigned short **sptr, int offset)
{
    unsigned int x = 5;

    **sptr = x;
    (*sptr) += offset;
    **sptr = x;
}

__attribute__ ((noinline))
void conditional1(volatile unsigned short *sptr, bool cond)
{
    unsigned int x = 5;

    *sptr = x;
    if (cond) {
        *sptr = x + 1;
    } else {
        *sptr = x + 2;
    }
}

__attribute__ ((noinline))
void stupid(volatile unsigned int *ptr, unsigned short int val)
{
    val += 16;
    *ptr = val;
}

__attribute__ ((noinline))
void l1_hll_interaction(int count, unsigned short l1p *out_data, unsigned short *in_data)
{
    for (int i = 0; i < count; i++) {
      out_data[i] = in_data[i];
    }
}
__attribute__ ((noinline))
unsigned short l1p * get_ptr()
{
    return (unsigned short l1p *)(4);
}

__attribute__ ((noinline))
void l1_hll_interaction2(int count, unsigned short *in_data)
{
    unsigned short l1p *out_data = get_ptr();
    for (int i = 0; i < count; i++) {
      out_data[i] = in_data[i];
    }
}

__attribute__ ((noinline))
void do_little(volatile unsigned char *ptr)
{
    unsigned char x = *ptr;
}

__attribute__ ((noinline))
int fn_call1(volatile unsigned char *ptr, bool val)
{
    *ptr = 5;
    do_little(ptr);
    if (val) {
        return 5;
    } else {
        return 4;
    }
}

__attribute__ ((noinline))
int fn_call2(volatile unsigned char *ptr, bool val)
{
    *ptr = 5;
    do_little(ptr);
    if (val) {
        volatile unsigned char x = *ptr;
    } else {
        volatile unsigned char x = *(ptr+1);
    }
}

#if 0
// The below is from gcc torture test
// My first implementation of the attrib chasing was too slow for this test,
// fixed using memoization.  Leaving this test here as it is good for finding
// this type of issue.  Disabled because it's...huge
char *bar (void);
__INTPTR_TYPE__ baz (void);
void
gcc_test_pr85180(__INTPTR_TYPE__ *q)
{
  char *p = bar ();
  __INTPTR_TYPE__ a = baz ();
  __INTPTR_TYPE__ b = baz ();
  int i = 0;
#define X q[i++] = a; q[i++] = b; a = a + b; b = b + a;
#define Y X X X X X X X X X X
#define Z Y Y Y Y Y Y Y Y Y Y
  Z Z Z Z Z Z Z Z Z Z
      p[a] = 1;
  p[b] = 2;
}
#endif

int main(int argc, char *argv[])
{
}
