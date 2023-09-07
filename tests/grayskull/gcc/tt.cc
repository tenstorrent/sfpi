/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ckernel_ops.h"

namespace ckernel {
    volatile unsigned int *instrn_buffer;
};
using namespace ckernel;


int main(int argc, char *argv[])
{
    TTI_ADDDMAREG(1, 2, 3, 4);
    TTI_ADDRCRXY(1, 2, 3, 4, 5, 6);
    TTI_ADDRCRZW(1, 2, 3, 4, 5, 6);
    TTI_APOOL3S1(3, 2, 1);
    TTI_APOOL3S2(3, 2, 1);
    TTI_ATCAS(1, 0, 3, 2, 5, 8);
    TTI_ATCAS(1, 2, 3, 0, 5, 6);
    TTI_ATGETM(1);
    TTI_ATINCGET(1, 2, 3, 4, 5);
    TTI_ATINCGETPTR(1, 0, 3, 4, 1, 6, 7);
    TTI_ATINCGETPTR(0, 1, 3, 4, 0, 6, 7);
    TTI_ATRELM(1);
    TTI_ATSWAP(1, 2, 3, 4);
    TTI_BITWOPDMAREG(1, 2, 3, 4, 5);
    TTI_CLEARDVALID(1, 2);
    TTI_CLREXPHIST;
    TTI_CMPDMAREG(1, 2, 3, 4, 5);
    TTI_CONV3S1(1, 2, 3, 4);
    TTI_CONV3S2(1, 2, 3, 4);
    TTI_DMANOP;
    TTI_DOTPV(0, 1, 2, 3);
    TTI_ELWADD(0, 1, 2, 3);
    TTI_ELWMUL(0, 1, 2, 3);
    TTI_ELWSUB(0, 1, 2, 3);
    TTI_FLUSHDMA(2);
    TTI_GAPOOL(4, 1, 2, 1);
    TTI_GATESRCRST(12, 1);
    TTI_GMPOOL(0, 2, 3, 1);
    TTI_INCADCXY(0, 1, 2, 3, 4);
    TTI_INCADCZW(0, 1, 2, 3, 4);
    TTI_INCRWC(0, 1, 2, 3);
    TTI_LOADIND(0, 1, 2, 3, 4);
    TTI_LOADREG(0, 4);
    TTI_MOP(0, 1, 2);
    TTI_MOP_CFG(1);
    TTI_MOVA2D(0, 1, 2, 3);
    TTI_MOVB2D(0, 1, 2, 3);
    TTI_MOVD2A(0, 1, 2, 3);
    TTI_MOVDBGA2D(0, 1, 2, 3);
    TTI_MPOOL3S1(2, 3, 1);
    TTI_MPOOL3S2(2, 3, 1);
    TTI_MULDMAREG(0, 1, 2, 3);
    TTI_MVMUL(0, 1, 2, 3);
    TTI_NOP;
    TTI_PACR(6, 2, 3, 1, 4, 5, 0);
    TTI_RAREB;
    TTI_RDCFG(0, 1);
    TTI_REG2FLOP(0, 1, 2, 3, 4, 5);
    TTI_RSTDMA;
    TTI_SEMGET(2);
    TTI_SEMINIT(1, 2, 3);
    TTI_SEMPOST(2);
    TTI_SEMWAIT(1, 2, 3);
    TTI_SETADC(2, 1, 3, 4);
    TTI_SETADCXX(1, 2, 3);
    TTI_SETADCXY(1, 2, 3, 4, 5, 6);
    TTI_SETADCZW(1, 2, 3, 4, 5, 6);
    TTI_SETASHRMH(2, 1);
    TTI_SETASHRMH0(2, 1);
    TTI_SETASHRMH1(2, 1);
    TTI_SETASHRMV(2);
    TTI_SETC16(1, 2);
    TTI_SETDMAREG(2, 3, 1, 4);
    TTI_SETDVALID(2);
    TTI_SETPKEDGOF(1, 2, 3, 4);
    TTI_SETRWC(1, 2, 3, 4, 5, 6);
    TTI_SFPABS(1, 2, 3, 4);
    TTI_SFPADD(1, 2, 3, 4, 5);
    TTI_SFPADDI(1, 2, 3);
    TTI_SFPAND(1, 2, 3, 4);
    TTI_SFPCOMPC(1, 2, 3, 4);
    TTI_SFPDIVP2(1, 2, 3, 4);
    TTI_SFPENCC(1, 2, 3, 4);
    TTI_SFPEXEXP(1, 2, 3, 4);
    TTI_SFPEXMAN(1, 2, 3, 4);
    TTI_SFPIADD(1, 2, 3, 4);
    TTI_SFPLOAD(1, 2, 3);
    TTI_SFPLOADI(1, 2, 3);
    TTI_SFPLUT(1, 2, 3);
    TTI_SFPLZ(1, 2, 3, 4);
    TTI_SFPMAD(1, 2, 3, 4, 5);
    TTI_SFPMOV(1, 2, 3, 4);
    TTI_SFPMUL(1, 2, 3, 4, 5);
    TTI_SFPMULI(1, 2, 3);
    TTI_SFPNOT(1, 2, 3, 4);
    TTI_SFPOR(1, 2, 3, 4);
    TTI_SFPPOPC(1, 2, 3, 4);
    TTI_SFPPUSHC(1, 2, 3, 4);
    TTI_SFPSETCC(1, 2, 3, 4);
    TTI_SFPSETEXP(1, 2, 3, 4);
    TTI_SFPSETMAN(1, 2, 3, 4);
    TTI_SFPSETSGN(1, 2, 3, 4);
    TTI_SFPSHFT(1, 2, 3, 4);
    TTI_SFPSTORE(1, 2, 3);
    TTI_SHIFTDMAREG(1, 2, 3, 4, 5);
    TTI_SHIFTXA(1, 2);
    TTI_STALLWAIT(1, 2);
    TTI_STOREIND(1, 0, 0, 2, 3, 4, 5);
    TTI_STOREIND(0, 1, 0, 2, 3, 4, 5);
    TTI_STOREIND(0, 0, 1, 2, 3, 4, 5);
    TTI_STOREREG(1, 2);
    TTI_SUBDMAREG(1, 2,3, 4);
    TTI_TRNSPSRCA;
    TTI_UNPACR(0, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    TTI_UNPACR(1, 2, 3, 4, 0, 1, 1, 1, 1, 1, 1, 1, 1);
    TTI_UNPACR(1, 2, 3, 4, 1, 0, 1, 1, 1, 1, 1, 1, 1);
    TTI_UNPACR(1, 2, 3, 4, 1, 1, 0, 1, 1, 1, 1, 1, 1);
    TTI_UNPACR(1, 2, 3, 4, 1, 1, 1, 0, 1, 1, 1, 1, 1);
    TTI_UNPACR(1, 2, 3, 4, 1, 1, 1, 1, 0, 1, 1, 1, 1);
    TTI_UNPACR(1, 2, 3, 4, 1, 1, 1, 1, 1, 0, 1, 1, 1);
    TTI_UNPACR(1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 0, 1, 1);
    TTI_UNPACR(1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 0, 1);
    TTI_UNPACR(1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    TTI_UNPACR_NOP(1, 2);
    TTI_WRCFG(2, 1, 3);
    TTI_XMOV(1, 2);
    TTI_ZEROACC(1, 2, 3);
    TTI_ZEROSRC(2, 1, 0, 1);
}
