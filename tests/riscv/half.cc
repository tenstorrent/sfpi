#include <cstdio>

volatile unsigned short *dst = (unsigned short *)0xffffeeee;
volatile unsigned int *dstw = (unsigned int *)0xffffeeee;
unsigned short *dstg = (unsigned short *)0xffffeeee;

void replace(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dst = tmp;
}

void replace2(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dst = tmp;
    *dst = tmp;
}

void replace3(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dst = tmp;
    *dst = tmp;
    *dst = tmp;
}

void replace_word_write(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    *dstw = tmp;
}

unsigned short keep_later_use(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dst = tmp;
    return tmp;
}

unsigned short keep_later_use2(unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    unsigned short tmp2 = tmp + 10;
    *dst = tmp;
    return tmp2;
}

unsigned short keep_later_use2(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dst = tmp;
    *dst = tmp;
    return tmp;
}

unsigned short keep_bb(volatile unsigned short *ptr, int count)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dst = tmp;

    for (int i = 0; i < count; i++) {
        *dst = 0;
    }

    return tmp;
}

void keep_word_write(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    *dstw = tmp;
}

void keep_contorted(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    volatile unsigned short *contort = (volatile unsigned short *)(int)tmp;

    *contort = tmp;
}

unsigned short keep_word_load_ret(volatile unsigned int *ptr)
{
    unsigned short tmp = *ptr;
    *dst = tmp;
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

void keep_param(volatile unsigned int *ptr, unsigned short *out)
{
    unsigned short tmp = *ptr;
    *out = tmp;
}

__attribute__ ((noinline)) void use_fn(unsigned int x)
{
    *dstw = x;
}

void replace_func_call(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    use_fn(tmp);
}

void keep_func_call(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    use_fn(tmp);
    *ptr = tmp;
}

void keep_func_call2(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 10;
    use_fn(tmp);
}

#define INSTRUCTION_WORD(x) __asm__ __volatile__(".word (%0)" : : "i" ((x)))

void keep_asm(volatile unsigned short *ptr)
{
    unsigned short tmp = *ptr;
    tmp += 5;
    INSTRUCTION_WORD(3);
    *dst = tmp;
}

int main(int argc, char *argv[])
{
    volatile unsigned short *in = (unsigned short *)0xffffeeee;
    volatile unsigned int *inw = (unsigned int *)0xffffeeee;
    unsigned short foo;

    replace(in);
    replace2(in);
    replace3(in);
    replace_word_write(in);
    keep_later_use(in);
    keep_later_use2(in);
    keep_bb(in, argc);
    keep_word_write(in);
    keep_contorted(in);
    keep_word_load_ret(inw);
    keep_ret(in);
    keep_ret2(in);
    keep_param(inw, &foo);
    replace_func_call(in);
    keep_func_call(in);
    keep_func_call2(in);
    keep_asm(in);
}
