/*
 * crt0-efi-loongarch64.S - PE/COFF header for LoongArch64 EFI applications
 *
 * Copyright (C) 2021 Loongson Technology Corporation Limited. <zhoumingtao@loongson.cn>
 * Copyright (C) 2014 Linaro Ltd. <ard.biesheuvel@linaro.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice and this list of conditions, without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 */

	.text
	.align		12
	.globl	_start
	.type	_start, @function
_start:
	addi.d	  $sp, $sp, -24
	st.d	  $ra, $sp, 0
	st.d	  $a0, $sp, 8
	st.d 	  $a1, $sp, 16

	move	  $a2, $a0		// a2: ImageHandle
	move	  $a3, $a1 		// a3: SystemTable
	la.local  $a0, ImageBase	// a0: ImageBase
	la.local  $a1, _DYNAMIC		// a1: DynamicSection
	bl        _relocate
	bnez	  $a0, 0f

	ld.d	  $a0, $sp, 8
	ld.d	  $a1, $sp, 16
	bl        _entry

0:	ld.d	  $ra, $sp, 0
	addi.d	  $sp, $sp, 24
	jr	      $ra

// hand-craft a dummy .reloc section so EFI knows it's a relocatable executable:

 	.data
dummy:	.4byte	0

#define IMAGE_REL_ABSOLUTE	0
 	.section .reloc, "a"
label1:
	.4byte	dummy-label1				// Page RVA
	.4byte	12					// Block Size (2*4+2*2), must be aligned by 32 Bits
	.2byte	(IMAGE_REL_ABSOLUTE<<12) +  0		// reloc for dummy
	.2byte	(IMAGE_REL_ABSOLUTE<<12) +  0		// reloc for dummy

#if defined(__ELF__) && defined(__linux__)
	.section .note.GNU-stack,"",%progbits
#endif
