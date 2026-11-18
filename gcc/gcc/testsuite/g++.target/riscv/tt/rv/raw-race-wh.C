// { dg-do compile }
// { dg-additional-options "-mcpu=tt-wh -mabi=ilp32 -fno-inline -O2 -fno-exceptions" }

// Read after write race with different sized accesses and different
// effective addresses.

template<typename T> union u {
  unsigned i[2];
  struct {
    T volatile a, b;
  } s;
};

// These are ordered nicely, we don't need the workaround (but emit it anyway).
// It'd be nice to do better.
template<typename T>
unsigned ok (unsigned x, unsigned y) {
    u<T> u;

    u.s.b = y;
    u.s.a = x;
    return u.i[0];
}

template<typename T>
unsigned bad (unsigned x, unsigned y) {
  u<T> u;

  u.s.a = x;
  u.s.b = y;
  return u.i[0];
}


int foo (unsigned x, unsigned y) {
  return ok<short> (x, y) + ok<char> (x, y)
    + bad<short> (x, y) + bad<char> (x, y);
}

// Ideally we'd not insert in these cases ...
// { dg-final { scan-assembler {_Z2okIsEjjj:\n[^:]*\n[\t ]+sh[\t ]+a.,8\(sp\)\n[\t ]+lhu[\t ]+zero,8\(sp\)\n[\t ]+lw[\t ]+a.,8\(sp\)\n} } }
// { dg-final { scan-assembler {_Z2okIcEjjj:\n[^:]*\n[\t ]+sb[\t ]+a.,8\(sp\)\n[\t ]+lbu[\t ]+zero,8\(sp\)\n[\t ]+lw[\t ]+a.,8\(sp\)\n} } }

// ... and generate this ...
// { dg-final { scan-assembler {_Z2okIsEjjj:\n[^:]*\n[\t ]+sh[\t ]+a.,8\(sp\)\n[\t ]+lw[\t ]+a.,8\(sp\)\n} { xfail *-*-* } } }
// { dg-final { scan-assembler {_Z2okIcEjjj:\n[^:]*\n[\t ]+sb[\t ]+a.,8\(sp\)\n[\t ]+lw[\t ]+a.,8\(sp\)\n} { xfail *-*-* } } }

// ... but thse do need the workaround
// { dg-final { scan-assembler {_Z3badIsEjjj:\n[^:]*\n[\t ]+sh[\t ]+a.,10\(sp\)\n[\t ]+lhu[\t ]+zero,10\(sp\)\n[\t ]+lw[\t ]+a.,8\(sp\)\n} } }
// { dg-final { scan-assembler {_Z3badIcEjjj:\n[^:]+[\t ]+sb[\t ]+a.,9\(sp\)\n[\t ]+lbu[\t ]+zero,9\(sp\)\n[\t ]+lw[\t ]+a.,8\(sp\)\n} } }
