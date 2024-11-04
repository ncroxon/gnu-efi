/* SPDX-License-Identifier: GPL-2.0-or-later OR BSD-2-Clause */
/*
 * Copyright (C) 2014 Linaro Ltd. <ard.biesheuvel@linaro.org>
 * Copyright (C) 2018 Alexander Graf <agraf@suse.de>
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
	.globl _start
	.type _start,%function
_start:
	addi		sp, sp, -24
	sd		a0, 0(sp)
	sd		a1, 8(sp)
	sd		ra, 16(sp)
	lla		a0, ImageBase
	lla		a1, _DYNAMIC
	call		_relocate
	bne		a0, zero, .L_exit
	ld		a1, 8(sp)
	ld		a0, 0(sp)
	call		_entry
	ld		ra, 16(sp)
.L_exit:
	addi		sp, sp, 24
	ret

// hand-craft a dummy .reloc section so EFI knows it's a relocatable executable:
 
 	.data
dummy0:	.4byte	0
dummy1:	.4byte	0

#define IMAGE_REL_ABSOLUTE	0
 	.section .reloc, "a", %progbits
	.4byte	dummy1 - dummy0				// Page RVA
	.4byte	12					// Block Size (2*4+2*2), must be aligned by 32 Bits
	.2byte	(IMAGE_REL_ABSOLUTE<<12) +  0		// reloc for dummy
	.2byte	(IMAGE_REL_ABSOLUTE<<12) +  0		// reloc for dummy

#if defined(__ELF__) && defined(__linux__)
	.section .note.GNU-stack,"",%progbits
#endif
