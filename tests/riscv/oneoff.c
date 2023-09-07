volatile unsigned char *dstuqi = (unsigned char *)0xffffeeee;
volatile unsigned short *dstuhi = (unsigned short *)0xffffeeee;

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

