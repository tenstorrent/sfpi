// This file contains tests for non-load latency (imul/idiv for now)

#include <cstdio>

int mul_latency_chain(int a, int b, int c, int d, int e, int f)
{
    int x = a * b;
    int y = c + d;
    int z = e + f;

    return x + y + z;
}

int div_latency_chain(int a, int b, int c, int d, int e, int f)
{
    int x = a / b;
    int y1 = c + d;
    int y2 = a + b;
    int y3 = a + c;
    int z = e + f;

    return x + y1 + y2 + y3 + z;
}

int mod_latency_chain(int a, int b, int c, int d, int e, int f)
{
    int x = a % b;
    int y1 = c + d;
    int y2 = a + b;
    int y3 = a + c;
    int z = e + f;

    return x + y1 + y2 + y3 + z;
}

int main(int argc, char *argv[])
{
    
}
