// { dg-options "-std=c++17 -ftt-no-dyninit" }

int foo ();

int A1 = 5;
thread_local int A2 = 6;

int B1 = foo (); // { dg-error "disallowed in this environment" }
thread_local int B2 = foo (); // { dg-error "disallowed in this environment" }

void baz ()
{
  static int A3 = 5;
  thread_local static int A4 = 5;

  static int B3 = foo (); // { dg-error "disallowed in this environment" }
  thread_local static int B4 = foo (); // { dg-error "disallowed in this environment" }
}


struct Dtor { ~Dtor (); };

Dtor G1; // { dg-error "disallowed in this environment" }
thread_local Dtor G2; // { dg-error "disallowed in this environment" }

void bar ()
{
  static Dtor G3; // { dg-error "disallowed in this environment" }
  thread_local static Dtor G4; // { dg-error "disallowed in this environment" }
}
