// This file contains tests for load latency and riscv L1 load intrinsic

#include <cstdio>

#define l1p __attribute__((rvtt_l1_ptr))
#define regp __attribute__((rvtt_reg_ptr))

struct __ts {
    int a;
    int b;
};

union __tu {
    l1p int *l1;
    int *ll;
};

struct __ar {
    int l1p v[5];
};

struct __arp {
    int *l1p v[5];
};

int global[4];
int l1p global_l1[4];

struct __ts l1p global_ts[4];

int l1p *global_l1_ptr = &global[0];
int l1p *global_l1_xo_ptr = &global_l1[0];
int l1p *global_l1_const_ptr = (int *)0x800;

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

int latency_chain_dependent_decorated(l1p int *p, int a, int b, int c, int d, int e)
{
    return *p + a + b + c + d + e;
}

int latency_chain_split_decorated(l1p int *p, int a, int b, int c, int d, int e)
{
    int x = *p + a;
    int y = b + c;
    int z = d + e;

    return x + y + z;
}

int load_global()
{
    return global[0];
}

int load_global_l1()
{
    return global_l1[0];
}

int load_global_l1_xo_ptr()
{
    return *global_l1_xo_ptr;
}

int load_global_l1_ptr()
{
    return *global_l1_ptr;
}

int load_global_l1_ts_ptr()
{
    return global_ts[1].b;
}

int load_union_l1(union __tu thing)
{
    return *thing.l1;
}

int load_union_ll(union __tu thing)
{
    return *thing.ll;
}

int load_array(struct __ar l1p *p)
{
    return p->v[1];
}

int load_array2(struct __ar l1p *p, int n)
{
    return p->v[n];
}

int load_arrayp(struct __arp l1p *p)
{
    return *p->v[0];
}

int load_arrayp2(struct __arp l1p *p)
{
    return *p->v[0] + *p->v[1] + *p->v[2] + *p->v[3] + *p->v[4];
}

int different(int l1p x)
{
    return x;
}

int load_global_l1_const()
{
    return *global_l1_const_ptr;
}

int cast1()
{
    return *(l1p int *)(0x320);
}

int cast2(int scale, int offset)
{
    return *(l1p int *)(0x320 + 4 * scale + offset);
}

int cast3(int value)
{
    return *(l1p int *)(value);
}

// Forces the MEM_EXPR to contain a PARM_DECL
int lots_o_args(int a0, int a1, int a2, int a3, int a4, int a5, 
                int a6, int a7, int a8, int a9, int a10, int a11, 
                int a12, int a13, int a14, int a15, int a16, int a17)
{
    return a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10 + a11 + a12 + a13 + a14 + a15 + a16 + a17;
}

// Forces the MEM_EXPR to contain a PARM_DECL
int lots_o_args(int a0, int a1, int a2, int a3, int a4, int a5, 
                int a6, int a7, int a8, int a9, int a10, int a11, 
                int a12, int a13, int a14, int a15, int a16, int l1p *a17)
{
    return a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10 + a11 + a12 + a13 + a14 + a15 + a16 + *a17;
}

int addr_calc_decorated1(l1p int *ptr)
{
    return *ptr;
}

int addr_calc_decorated2(l1p int *ptr)
{
    return *(ptr + 5);
}

int addr_calc_decorated3(l1p struct __ts *ptr)
{
    return ptr->a;
}

int addr_calc_decorated4a(l1p struct __ts *ptr)
{
    return ptr->b;
}

int addr_calc_decorated4b(l1p struct __ts *ptr)
{
    return ptr[5].b;
}

int addr_calc_decorated5(volatile l1p int *ptr)
{
    return *ptr;
}

int plus(l1p struct __ts *ptr)
{
    return ptr->b + 5;
}

unsigned char uqi(l1p unsigned char *ptr)
{
    return *ptr;
}

char qi(l1p char *ptr)
{
    return *ptr;
}

unsigned short uhi(l1p unsigned short *ptr)
{
    return *ptr;
}

short hi(l1p short *ptr)
{
    return *ptr;
}

// coercion up
int up_qi(l1p char *ptr)
{
    return *ptr;
}

// coercion up
unsigned int up_uqi(l1p unsigned char *ptr)
{
    return *ptr;
}

// coercion up
int up_hi(l1p short *ptr)
{
    return *ptr;
}

// coercion up
unsigned int up_uhi(l1p unsigned short *ptr)
{
    return *ptr;
}

// coercion down
char down_qi(l1p int *ptr)
{
    return *ptr;
}

// coercion down
unsigned char down_uqi(l1p unsigned int *ptr)
{
    return *ptr;
}

// coercion down
short down_hi(l1p int *ptr)
{
    return *ptr;
}

// coercion down
unsigned short down_uhi(l1p unsigned int *ptr)
{
    return *ptr;
}

volatile l1p int *l1ptrtestreturn(volatile l1p int *l1ptr)
{
    return l1ptr;
}

// wish these caused an error
int * error_drop_attribute_on_return(l1p int *ptr)
{
    return ptr;
}

int error_drop_attribute_on_assign(l1p int *ptr)
{
    int *ptr1 = ptr;
    return *ptr1;
}

l1p int * cast(int *ptr)
{
    return (l1p int *)ptr;
}

void store(l1p int *ptr)
{
    *ptr = 0;
}

void copy(l1p struct __ts *ptr)
{
    ptr->b = ptr->a;
}

// wish the following caused errors
l1p int * error_add_attribute_on_return(int *ptr)
{
    return ptr;
}

l1p int * error_add_attribute_on_assign(int *ptr)
{
    l1p int *ptr1 = ptr;
    return ptr1;
}

int error_l1_ptr_type_param1(l1p int l1ptr)
{
    return l1ptr;
}

int error_l1_ptr_type_param2(volatile l1p int l1ptr)
{
    return l1ptr;
}

int error_l1_ptr_type_var()
{
    l1p int l1ptr = 1;
    return l1ptr;
}

l1p int error_l1_ptr_return(volatile l1p int *l1ptr)
{
    return *l1ptr;
}

unsigned int reg_read_latency(int regp *p, int a, int b, int c, int d, int e)
{
    int x = *p + a;
    int y = b + c;
    int z = d + e;

    return x + y + z;
}

void many_uses(l1p int *l1, int *ll)
{
    int c = ll[0];
    int a = l1[0];
    int b = a + a;
    b += a;
    b += c;
    ll[0] = a;
    ll[1] = b;
}

int hoist(l1p int *l1, int *ll)
{
    int a = ll[0];
    int b = ll[1];
    int c = ll[2];
    int d = l1[0];

    return a + b + c + d;
}

int hoist_store(l1p int *l1, int *ll)
{
    int a = ll[0];
    ll[0] = 3;
    int b = ll[1];
    int c = ll[2];
    int d = l1[0];

    return a + b + c + d;
}

int hoist_restrict_store(l1p int *l1, int * __restrict__ ll, int idx)
{
    int a = ll[idx];
    ll[0] = 3;
    int b = ll[idx+1];
    int c = ll[idx+2];
    int d = l1[0];

    return a + b + c + d;
}

int hoist_volatile_loads(l1p int *l1, int volatile * ll)
{
    int a = ll[0];
    int b = ll[1];
    int c = ll[2];
    int d = l1[0];

    return a + b + c + d;
}

int hoist_volatile_restrict_load(l1p int *l1, int volatile * __restrict__ ll)
{
    int a = ll[0];
    ll[0] = 3;
    int b = ll[1];
    int c = ll[2];
    int d = l1[0];

    return a + b + c + d;
}

int hoist_unspec_volatile(l1p int *l1, int* ll)
{
    int a = ll[0];
    __builtin_rvtt_gs_sfppushc();
    __builtin_rvtt_gs_sfppopc();
    int b = ll[1];
    int c = ll[2];
    int d = l1[0];

    return a + b + c + d;
}

int lower(l1p int *l1, int* ll, int offset)
{
    int a = ll[0];
    int b = ll[1];
    int c = ll[2];
    int d = l1[0];
    int e = c + d;
    int f = ll[offset];

    return a + b + e + f;
}

int lower2(l1p int *l1, int* ll, int offset)
{
    int a = ll[0];
    int b = ll[1];
    int c = ll[2];
    int d = l1[0];
    int e = c + d;
    int f = ll[offset];
    int g = e >> 1;

    return a + b + e + f + g;
}

unsigned int real_test_case(unsigned int noc, unsigned int reg_id, int a, int b, int c, int d, int e)
{
    unsigned int offset = (noc << 16) + reg_id;
    volatile unsigned int regp * ptr = (volatile unsigned int regp *)offset;
    return *ptr + a + b + c + d + e;
}

int main(int argc, char *argv[])
{
}
