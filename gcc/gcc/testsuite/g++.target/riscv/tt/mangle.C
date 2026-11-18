
// ICE with mangling due to confusion about whether the attribute
// creates a distinct type or not.

struct X {};

void Foo (X __attribute__((rvtt_l1_ptr)) *p) {}
// { dg-final { scan-assembler {\n_Z3FooU11rvtt_l1_ptrP1X:} } }

void Bar (X __attribute__((rvtt_l1_ptr)) *p, X __attribute__((rvtt_l1_ptr)) *q) {}
// { dg-final { scan-assembler {\n_Z3BarU11rvtt_l1_ptrP1XS1_:} } }

void Baz (X __attribute__((rvtt_l1_ptr)) *, X __attribute__((rvtt_l1_ptr)) *) {}
// { dg-final { scan-assembler {\n_Z3BazU11rvtt_l1_ptrP1XS1_:} } }

void Quux (X *p, X __attribute__((rvtt_l1_ptr)) *q) {}
// { dg-final { scan-assembler {\n_Z4QuuxP1XU11rvtt_l1_ptrS0_:} } }
