namespace ckernel{
    unsigned *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void gt () {
  vFloat a = dst_reg[0];
  vInt b = dst_reg[1];

  v_if (b > 0) {
    a = 0;
  } v_endif;
  

  dst_reg[0] = a;
}

void lte () {
  vFloat a = dst_reg[0];
  vInt b = dst_reg[1];

  v_if (b <= 0) {
    a = 0;
  } v_endif;
  
  dst_reg[0] = a;
}
