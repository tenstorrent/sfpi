// Test the compiler's implementation of the workaround for the GS
// L1/local_memory arbiter bug

__attribute__ ((noinline))
int lotsoparams(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k)
{
    return a + b + c + d + e + f + g + h + i + j + k;
}

__attribute__ ((noinline))
int simple(int *l1, int *lm)
{
    return __builtin_rvtt_l1_load_si(*l1) + lm[0] + lm[2] + lm[3] + lm[4] + lm[5];
}

void copy(int *l1, int *lm)
{
#pragma GCC unroll 15
    for (int i = 0; i < 15; i++) {
        *(lm + i) = __builtin_rvtt_l1_load_si(*(l1 + i)) + *(lm + i);
    }
}

__attribute__ ((noinline))
int double_load(int *l1, int *lm)
{
    return __builtin_rvtt_l1_load_si(*l1) + lm[0] + lm[2] + __builtin_rvtt_l1_load_si(*(l1+1)) + lm[3] + lm[4] + lm[5];
}

int call1(int *l1, int *lm)
{
    int tmp = __builtin_rvtt_l1_load_si(*l1);
    return simple(l1, lm) + tmp;
}

int call2(int *l1, int *lm)
{
    int tmp = __builtin_rvtt_l1_load_si(*l1);
    return lotsoparams(tmp, 1, 2, 3, 4, 5, 6, 7 , 8, 9, 10);
}

int call3(int *l1, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j)
{
    // Compiler orders things such that this test isn't so interesting, the lw
    // off the stack come early
    volatile int tmp = __builtin_rvtt_l1_load_si(*l1);
    return lotsoparams(tmp, j, i, h, g, f, e, d, c, b, a);
}

int at_exit(int *l1, int *lm)
{
    return __builtin_rvtt_l1_load_si(*l1);
}

int loop1(int *l1, int *lm)
{
    int tmp = __builtin_rvtt_l1_load_si(*l1);

    int sum = 0;
#pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        sum += lm[i];
    }

    return tmp + sum;
}

int loop1_preempt(int *l1, int *lm)
{
    int tmp = __builtin_rvtt_l1_load_si(*l1);
    __builtin_rvtt_gs_l1_load_war(tmp);

    int sum = 0;
#pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        sum += lm[i];
    }

    return tmp + sum;
}

int loop2(int *l1, int *lm)
{
    int sum = 0;
    int tmp = 0;
#pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        tmp += __builtin_rvtt_l1_load_si(*l1++);
        sum += lm[i];
    }

    return tmp + sum;
}

int loop3(int *l1, int *lm)
{
    int tmp = __builtin_rvtt_l1_load_si(*l1);

    int sum = 0;
#pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        if (lm[i] == lm[i + 10]) {
            sum += lm[i];
        } else {
            sum += lm[i + 1];
        }
    }

    return tmp + sum;
}

int loop3_preempt(int *l1, int *lm)
{
    int tmp = __builtin_rvtt_l1_load_si(*l1);
    __builtin_rvtt_gs_l1_load_war(tmp);

    int sum = 0;
#pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        if (lm[i] == lm[i + 10]) {
            sum += lm[i];
        } else {
            sum += lm[i + 1];
        }
    }

    return tmp + sum;
}

int loop4(int *l1, int *lm)
{
    int sum = 0;
    int tmp = 0;
#pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        if (lm[i] == lm[i + 10]) {
            tmp += __builtin_rvtt_l1_load_si(*l1++);
        } else {
            sum += lm[i];
        }
    }

    return tmp + sum;
}

int loop5(int *l1, int *lm)
{
    int sum = 0;
    int tmp = 0;
#pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        int val;
        if (lm[i] == lm[i + 10]) {
            val = __builtin_rvtt_l1_load_si(*l1++);
        } else {
            val = lm[i];
        }
        sum += val;
    }

    return tmp + sum;
}

int cond1(int *l1, int *lm, bool c)
{
    int tmp = 0;
    if (c) {
        tmp = __builtin_rvtt_l1_load_si(*l1);
    } else {
        tmp = *lm;
    }    
    return tmp + *(lm + 1);
}

int cond2(int *l1, int *lm, bool c)
{
    int tmp = __builtin_rvtt_l1_load_si(*l1);
    if (c) {
        tmp += *lm;
    } else {
        tmp += *(lm + 1) + *(lm + 2) + *(lm + 3) + *(lm + 4);
    }    
    return tmp;
}

int cond3(int *l1, int *lm, bool c)
{
    int tmp = __builtin_rvtt_l1_load_si(*l1);
    if (c) {
        tmp += *lm;
    } else {
        tmp += *(lm + 1) + *(lm + 2) + *(lm + 3) + *(lm + 4) + *(lm + 5);
    }    
    return tmp;
}

int complex(int *l1, int *lm, bool c)
{
    int tmp = __builtin_rvtt_l1_load_si(*l1);
    if (c) {
        tmp += lm[0];
    }
    
    if (tmp > 5) {
        tmp += lm[1];
    }

    for (int i = 0; i < tmp; i++) {
        tmp += lm[i + 1];

        if (tmp > 33) {
            tmp += __builtin_rvtt_l1_load_si(l1[i]);
        } else {
            return tmp;
        }
    }

    return tmp;
}

int complex2(int *l1, int *lm, int m, int n)
{
    int tmp = __builtin_rvtt_l1_load_si(*l1);
    if (m) {
        tmp += lm[0];
    }
    
    if (tmp > 5) {
        tmp += lm[1];
    }

    for (int i = 0; i < tmp; i++) {
        tmp += lm[i + 1];

        int loaded = lm[i*2];
        if (tmp & m) {
            loaded = __builtin_rvtt_l1_load_si(l1[i]);
        } else {
            return tmp;
        }
        if (tmp & n) {
            tmp += loaded;
        }
    }

    return tmp;
}

void remove_war1()
{
    int tmp = 0;
    __builtin_rvtt_gs_l1_load_war(tmp);
}

void remove_war2()
{
    int tmp = 0;
    __builtin_rvtt_gs_l1_load_war(tmp);
    __builtin_rvtt_gs_l1_load_war(tmp);
}

void remove_war3()
{
    int tmp = 0;
    __builtin_rvtt_gs_l1_load_war(tmp);
    __builtin_rvtt_gs_l1_load_war(tmp);
    __builtin_rvtt_gs_l1_load_war(tmp);
}

int remove_war4(int *l1, int *lm)
{
    int a = __builtin_rvtt_l1_load_si(*l1);
    int b = lm[0] + lm[2] + lm[3] + lm[4] + lm[5] + a;
    __builtin_rvtt_gs_l1_load_war(a);

    return b;
}

int cancel1(int *l1, int *lm)
{
    int a = __builtin_rvtt_l1_load_si(*l1);
    int b = a + lm[0] + lm[2] + lm[3] + lm[4];

    return b;
}

int cancel2(int *l1, int *lm, int *out)
{
    int sum = 0;
    for (int i = 0; i < 6; i++) {
        out[i] = __builtin_rvtt_l1_load_si(l1[i]) + lm[i];
    }
    return sum;
}

int cancel3(int *l1, int *lm, int *out)
{
    int sum = 0;
    for (int i = 0; i < 6; i++) {
        int partial = 0;
        sum += __builtin_rvtt_l1_load_si(l1[i]);
        for (int j = 0; j < i; j++) {
            partial += lm[i * 6 + j];
        }
        sum += partial;
    }
    return sum;
}

int war_at_exit(int *l1)
{
    return __builtin_rvtt_l1_load_si(*l1);
}

// Ugh, pathological.  The compiler emits an l1 load w/o the intrinsic and
// stores the result on the stack then uses the intrinsic to load from the
// stack.  Following tests cover the "reorder" pass which reorders the loads
// for the different intrnisic types
int split_bb1_si(int *l1, int *ll, bool which)
{
    int a, b;
    if (which) {
        a = __builtin_rvtt_l1_load_si(l1[0]);
        b = ll[0];
    } else {
        a = ll[0];
        b = __builtin_rvtt_l1_load_si(l1[0]);
    }

    return ll[3] + ll[4] + ll[5] + ll[6] + ll[7] + a + b;
}

int split_bb1_usi(unsigned int *l1, int *ll, bool which)
{
    int a, b;
    if (which) {
        a = __builtin_rvtt_l1_load_usi(l1[0]);
        b = ll[0];
    } else {
        a = ll[0];
        b = __builtin_rvtt_l1_load_usi(l1[0]);
    }

    return ll[3] + ll[4] + ll[5] + ll[6] + ll[7] + a + b;
}

int split_bb1_hi(short int *l1, int *ll, bool which)
{
    int a, b;
    if (which) {
        a = __builtin_rvtt_l1_load_hi(l1[0]);
        b = ll[0];
    } else {
        a = ll[0];
        b = __builtin_rvtt_l1_load_hi(l1[0]);
    }    

    return ll[3] + ll[4] + ll[5] + ll[6] + ll[7] + a + b;
}

int split_bb1_uhi(unsigned short int *l1, int *ll, bool which)
{
    int a, b;
    if (which) {
        a = __builtin_rvtt_l1_load_uhi(l1[0]);
        b = ll[0];
    } else {
        a = ll[0];
        b = __builtin_rvtt_l1_load_uhi(l1[0]);
    }

    return ll[3] + ll[4] + ll[5] + ll[6] + ll[7] + a + b;
}

int split_bb1_qi(char *l1, int *ll, bool which)
{
    int a, b;
    if (which) {
        a = __builtin_rvtt_l1_load_qi(l1[0]);
        b = ll[0];
    } else {
        a = ll[0];
        b = __builtin_rvtt_l1_load_qi(l1[0]);
    }

    return ll[3] + ll[4] + ll[5] + ll[6] + ll[7] + a + b;
}

int split_bb1_uqi(unsigned char *l1, int *ll, bool which)
{
    int a, b;
    if (which) {
        a = __builtin_rvtt_l1_load_uqi(l1[0]);
        b = ll[0];
    } else {
        a = ll[0];
        b = __builtin_rvtt_l1_load_uqi(l1[0]);
    }

    return ll[3] + ll[4] + ll[5] + ll[6] + ll[7] + a + b;
}

int split_bb1_ptr(unsigned int **l1, int **ll, bool which)
{
    int *a, *b;
    if (which) {
        a = (int *)__builtin_rvtt_l1_load_ptr(l1[0]);
        b = ll[0];
    } else {
        a = ll[0];
        b = (int *)__builtin_rvtt_l1_load_ptr(l1[0]);
    }

    return *ll[3] + *ll[4] + *ll[5] + *ll[6] + *ll[7] + *a + *b;
}

int split_bb1_baseline(volatile int *l1, volatile int *ll, bool which)
{
    int a, b;
    if (which) {
        a = l1[0];
        b = ll[0];
    } else {
        a = ll[0];
        b = l1[0];
    }

    return ll[3] + ll[4] + ll[5] + ll[6] + ll[7] + a + b;
}

int split_bb2(int *l1, int *ll, bool which)
{
    int a, b, c;
    c = 0;
    a = __builtin_rvtt_l1_load_si(l1[0]);
    b = ll[0];
    if (which) {
        c = __builtin_rvtt_l1_load_si(l1[1]);
    }

    return ll[3] + ll[4] + ll[5] + ll[6] + ll[7] + a + b + c;
}

#if 0
int error()
{
    int a;
    return __builtin_rvtt_l1_load_si(a);
}
#endif
// warning on WAR manually applied to the wrong value
int warning(int *l1, int *ll)
{
    int a = __builtin_rvtt_l1_load_si(l1[0]);
    int b = __builtin_rvtt_l1_load_si(l1[1]);
    int c = ll[0];
   __builtin_rvtt_gs_l1_load_war(a);

   return a + b + c;
}

int main(int argc, char *argv[])
{
    
}
