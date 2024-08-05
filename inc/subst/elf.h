/* Copyright (C) 2024 GNU-EFI Contributors

This library is free software;
you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation;
either version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library;
if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#if !defined(__GNU_EFI_NO_GLIBC)
#include <elf.h>

#else

typedef uint32_t Elf32_Addr;
typedef int32_t Elf32_Sword;
typedef uint32_t Elf32_Word;

typedef uint64_t Elf64_Addr;
typedef int64_t Elf64_Sxword;
typedef uint64_t Elf64_Xword;


typedef struct {
               Elf32_Addr   r_offset;
               Elf32_Word   r_info;
           } Elf32_Rel;

typedef struct {
               Elf64_Addr   r_offset;
               Elf64_Xword  r_info;
               Elf64_Sxword r_addend;
           } Elf64_Rela;

typedef struct {
               Elf32_Sword    d_tag;
               union {
                   Elf32_Word d_val;
                   Elf32_Addr d_ptr;
               } d_un;
           } Elf32_Dyn;

typedef struct {
               Elf64_Sxword    d_tag;
               union {
                   Elf64_Xword d_val;
                   Elf64_Addr  d_ptr;
               } d_un;
           } Elf64_Dyn;

#define ELF32_R_TYPE(val)		((val) & 0xff)
#define ELF64_R_TYPE(i)			((i) & 0xffffffff)

#define DT_NULL		0
#define DT_REL		17
#define DT_RELSZ	18
#define DT_RELENT	19
#define DT_RELA		7
#define DT_RELASZ	8
#define DT_RELAENT	9
#define DT_PLTGOT	3


#define R_AARCH64_NONE 0
#define R_AARCH64_RELATIVE     1027
#define R_RISCV_RELATIVE	 3
#define R_LARCH_NONE	0
#define R_LARCH_RELATIVE	3
#define R_X86_64_NONE	0
#define R_X86_64_RELATIVE	8
#define R_ARM_NONE		0
#define R_ARM_RELATIVE		23
#define R_386_NONE	   0
#define R_386_RELATIVE	   8
#define R_MIPS_NONE		0
#define R_MIPS_64		18
#define R_MIPS_REL32		3


#endif
