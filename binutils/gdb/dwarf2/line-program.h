/* DWARF 2 debugging format support for GDB.

   Copyright (C) 1994-2026 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef GDB_DWARF2_LINE_PROGRAM_H
#define GDB_DWARF2_LINE_PROGRAM_H

/* Decode the Line Number Program (LNP) for the line_header structure in
   CU.

   LOWPC is the lowest address in CU (or 0 if not known).  */

extern void dwarf_decode_lines (struct dwarf2_cu *cu, unrelocated_addr lowpc);

#endif /* GDB_DWARF2_LINE_PROGRAM_H */
