;; Machine description for Tenstorrent SFPU Intrinsics.
;; Copyright (C) 2022-2026 Tenstorrent Inc.
;; Originated by Ayonam Ray (ayonam@helprack.com)
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

(define_predicate "cstlreg_operand"
  (and (match_code "unspec")
       (match_test "XINT (op, 1) == UNSPEC_SFPCSTLREG")))

(define_predicate "noval_operand"
  (and (match_code "unspec")
       (match_test "XINT (op, 1) == UNSPEC_SFPNOVAL")))

(define_predicate "omit_operand"
  (and (match_code "unspec")
       (match_test "XINT (op, 1) == UNSPEC_SFPOMIT")))

(define_predicate "noval_or_omit_operand"
  (ior (match_operand 0 "noval_operand")
       (match_operand 0 "omit_operand")))

(define_predicate "reg_or_cstlreg_operand"
  (ior (match_operand 0 "cstlreg_operand")
       (match_operand 0 "register_operand")))

(define_predicate "reg_or_cstlreg_or_noval_operand"
  (ior (match_operand 0 "noval_operand")
       (match_operand 0 "reg_or_cstlreg_operand")))

(define_predicate "reg_or_cstlreg_or_noval_or_omit_operand"
  (ior (match_operand 0 "noval_operand")
       (match_operand 0 "omit_operand")
       (match_operand 0 "reg_or_cstlreg_operand")))

(define_predicate "mem_or_0_operand"
  (ior (match_operand 0 "const_0_operand")
       (match_operand 0 "memory_operand")))

(define_predicate "nonimmediate_or_cstlreg_operand"
  (ior (match_operand 0 "nonimmediate_operand")
       (match_operand 0 "cstlreg_operand")))

(define_predicate "const_int_or_noval_operand"
  (ior (match_operand 0 "const_int_operand")
       (match_operand 0 "noval_operand")))
