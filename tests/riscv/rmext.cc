#include <cstdio>

volatile unsigned char *dstuqi = (unsigned char *)0xffffeeee;
volatile unsigned short *dstuhi = (unsigned short *)0xffffeeee;
volatile unsigned int *dstusi = (unsigned int *)0xffffeeee;
unsigned short *dstg = (unsigned short *)0xffffeeee;

unsigned int load_baseline_uhi(unsigned short *ptr)
{
    return *ptr;
}

unsigned int load_baseline_uqi(unsigned char *ptr)
{
    return *ptr;
}

int load_baseline_hi(short *ptr)
{
    return *ptr;
}

// Looks like a GCC bug not sign extending this...
int load_baseline_uqi(char *ptr)
{
    return *ptr;
}

unsigned int load_uhi(volatile unsigned short *ptr)
{
    return *ptr;
}

unsigned int load_uqi(volatile unsigned char *ptr)
{
    return *ptr;
}

int load_hi(volatile short *ptr)
{
    return *ptr;
}

int load_qi(volatile char *ptr)
{
    return *ptr;
}

void store_baseline_uhi(unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstuhi = tmp;
}

void store_uhi(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstuhi = tmp;
}

void store_uqi(volatile unsigned char *ptr)
{
    unsigned char tmp = *ptr;
    tmp += 10;
    *dstuqi = tmp;
}

void keep_uqi(volatile unsigned char *ptr)
{
    unsigned char tmp = *ptr;
    tmp += 10;
    // Store into short, need the masking
    *dstuhi = tmp;
}

void double_store_uhi(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstuhi = tmp;
    *dstuhi = tmp;
}

unsigned short double_store_uhi_ret(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstuhi = tmp;
    *dstuhi = tmp;
    return tmp;
}

void double_store_uhi_use(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstuhi = tmp;
    *dstuhi = tmp;
    *dstuhi = tmp + 5;
}

void double_store_uhi_use2(volatile unsigned short *ptr, unsigned short a)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstuhi = tmp;
    *dstuhi = tmp;
    *dstuhi = tmp + a;
}

void double_store_uqi(volatile unsigned char *ptr)
{
    unsigned char tmp = *ptr;
    tmp += 10;
    *dstuqi = tmp;
    *dstuqi = tmp;
}

void triple_store_uhi(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstuhi = tmp;
    *dstuhi = tmp;
    *dstuhi = tmp;
}

void triple_store_uqi(volatile unsigned char *ptr)
{
    unsigned char tmp = *ptr;
    tmp += 10;
    *dstuqi = tmp;
    *dstuqi = tmp;
    *dstuqi = tmp;
}

void replace_write_uhi(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    *dstusi = tmp;
}

void replace_write_uqi(volatile unsigned char *ptr)
{
    unsigned char tmp = *ptr;
    *dstusi = tmp;
}

unsigned short keep_later_use(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstuhi = tmp;
    return tmp;
}

unsigned short keep_later_use2(unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    unsigned short tmp2 = tmp + 10;
    *dstuhi = tmp;
    return tmp2;
}

unsigned short keep_later_use3(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstuhi = tmp;
    *dstuhi = tmp;
    return tmp;
}

unsigned short keep_bb(volatile unsigned short *ptr, int count)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstuhi = tmp;

    for (int i = 0; i < count; i++) {
        *dstuhi = 0;
    }

    return tmp;
}

void keep_word_write(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstusi = tmp;
}

void keep_word_write(volatile unsigned char *ptr)
{
    unsigned char tmp = *ptr;
    tmp += 10;
    *dstusi = tmp;
}

void replace_contorted(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    volatile unsigned short *contort = (volatile unsigned short *)(int)tmp;

    *contort = tmp;
}

void keep_word_load(volatile unsigned int *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 2;
    *dstuhi = tmp;
}

void replace_word_load_store(volatile unsigned int *ptr)
{
    unsigned short tmp = *ptr;
    *dstuhi = tmp;
}

unsigned short keep_word_load_ret(volatile unsigned int *ptr)
{
    unsigned short tmp = *ptr;
    *dstuhi = tmp;
    return tmp;
}

unsigned short keep_ret(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    return tmp + 5;
}

unsigned short keep_ret2(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    unsigned short tmp2 = *ptr;
    unsigned short out = tmp + tmp2;
    out += *ptr;
    return out;
}

void replace_param(volatile unsigned int *ptr, unsigned short *out)
{
    unsigned short tmp = *ptr;
    *out = tmp;
}

// This shows up in dram_stream_intf.cc:
//   curr_dram_output_stream_state->r_dim_count = r_dim_count;
int real_world(volatile unsigned short *ptr)
{
    unsigned int v = *ptr;
    v++;
    *ptr = v;
    return (v == *(ptr+1)) ? 1 : 0;
}

// zero/sign extend followed by shift
// fold shift into extend operation
unsigned int mask_and_shift(unsigned short x)
{
    x += 1;
    return x << 4;
}

unsigned int mask_and_shift2(unsigned short x)
{
    x += 1;
    return (x << 4) + x;
}

int mask_and_shift3(short x)
{
    x += 1;
    return x << 4;
}

int mask_and_shift4(short x)
{
    x += 1;
    return (x << 4) + x;
}

unsigned short mask_and_shift5(unsigned short x)
{
    x += 1;
    return x << 4;
}

unsigned short mask_and_shift6(unsigned short x)
{
    x += 1;
    return (x << 4) + x;
}

short mask_and_shift7(short x)
{
    x += 1;
    return x << 4;
}

short mask_and_shift8(short x)
{
    x += 1;
    return (x << 4) + x;
}

int main(int argc, char *argv[])
{
    volatile unsigned char *inuqi = (unsigned char *)0xffffeeee;
    volatile char *inqi = (char *)0xffffeeee;
    volatile unsigned short *inuhi = (unsigned short *)0xffffeeee;
    volatile short *inhi = (short *)0xffffeeee;
    volatile unsigned int *inusi = (unsigned int *)0xffffeeee;
    volatile int *insi = (int *)0xffffeeee;
    unsigned short foo;

    load_uhi(inuhi);
    load_uqi(inuqi);
    load_hi(inhi);
    load_qi(inqi);
    store_uhi(inuhi);
    store_uqi(inuqi);
    keep_uqi(inuqi);
    double_store_uhi(inuhi);
    double_store_uqi(inuqi);
    replace_write_uhi(inuhi);
    replace_write_uqi(inuqi);
    keep_later_use(inuhi);
    keep_later_use2((unsigned short *)inuhi);
    keep_later_use3(inuhi);
    keep_bb(inuhi, argc);
}
