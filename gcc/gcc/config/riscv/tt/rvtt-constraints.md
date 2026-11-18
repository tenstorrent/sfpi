;; Machine description for Tenstorrent SFPU Blackhole Intrinsics.
;; Copyright (C) 2022-2026 Tenstorrent Inc.
;; Originated by Paul Keller (pkeller@tenstorrent.com)
;; Rewritten by Nathan Sidwell (nsidwell@tenstorrent.com, nathan@acm.org).

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; Register constraints

(define_register_constraint "xr" "SFPU_VAR_REGS"
  "Any of the variable SFPU registers")

;; specific allocatable registers
(define_register_constraint "x0" "SFPU_REGS_L0"
  "SFPU register L0")

(define_register_constraint "x1" "SFPU_REGS_L1"
  "SFPU register L1")

(define_register_constraint "x2" "SFPU_REGS_L2"
  "SFPU register L2")

(define_register_constraint "x3" "SFPU_REGS_L3"
  "SFPU register L3")

(define_register_constraint "x4" "SFPU_REGS_L4"
  "SFPU register L4")

(define_register_constraint "x5" "SFPU_REGS_L5"
  "SFPU register L5")

(define_register_constraint "x6" "SFPU_REGS_L6"
  "SFPU register L6")

(define_register_constraint "x7" "SFPU_REGS_L7"
  "SFPU register L7")

;; General constraints
(define_constraint "xc"
  "Any of the constant SFPU registers"
  (match_operand 0 "cstlreg_operand"))

(define_constraint "xs"
  "Any of non-sideways loading constant SFPU registers"
  (and (match_operand 0 "cstlreg_operand")
       (match_test "INTVAL (XVECEXP (op, 0, 0)) < 12")))

;; used for unused vector inputs
(define_constraint "xn"
  "No value"
  (match_operand 0 "noval_operand"))

(define_constraint "xo"
  "Omitted value"
  (match_operand 0 "omit_operand"))
