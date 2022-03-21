// Test LUT
// Early version had out of registers problem triggered by this

typedef float v64sf __attribute__((vector_size(64*4)));

int main(int argc, char* argv[])
{
    v64sf l0, l1, l2, l3;

    l0 = __builtin_rvtt_wh_sfploadi_ex(nullptr, 0, 12);
    l1 = __builtin_rvtt_wh_sfploadi_ex(nullptr, 0, 13);
    l2 = __builtin_rvtt_wh_sfploadi_ex(nullptr, 0, 14);
    l3 = __builtin_rvtt_wh_sfploadi_ex(nullptr, 0, 6);
    l3 = __builtin_rvtt_wh_sfplut(l1, l2, l0, l3, 5);
    l3 = __builtin_rvtt_wh_sfplut(l1, l2, l0, l3, 5);
}
