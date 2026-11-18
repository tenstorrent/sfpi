
// the rvtt_l1_ptr attrib creates a distinct type

extern int a; // { dg-message "previous declaration" }
extern int __attribute__((rvtt_l1_ptr)) a; // { dg-error "conflicting declaration" }}

extern int __attribute__((rvtt_l1_ptr)) *b; // { dg-message "previous declaration" }
extern int *b; // { dg-error "conflicting declaration" }}

void Frob (int *);
void Frob (int __attribute__((rvtt_l1_ptr)) *) {}


