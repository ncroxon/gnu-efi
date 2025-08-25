/*
 * Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.
 * This program and the accompanying materials are licensed and made 
available
 * under the terms and conditions of the BSD License which accompanies 
this
 * distribution.  The full text of the license may be found at
 * http://opensource.org/licenses/bsd-license.php.
 *
 * THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" 
BASIS,
 * WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED.
 */
	.text
	.globl	base_setjmp
#ifndef __MINGW32__
	.type base_setjmp, @function
#else
	.def base_setjmp; .scl 2; .type 32; .endef
#endif
base_setjmp:
	pop	%ecx
	movl	(%esp), %edx
	movl	%ebx, (%edx)
	movl	%esi, 4(%edx)
	movl	%edi, 8(%edx)
	movl	%ebp, 12(%edx)
	movl	%esp, 16(%edx)
	xorl	%eax, %eax
	jmp	*%ecx

	.globl	base_longjmp
#ifndef __MINGW32__
	.type	base_longjmp, @function
#else
	.def base_longjmp; .scl 2; .type 32; .endef
#endif
base_longjmp:
	pop	%eax
	pop	%edx
	pop	%eax
	movl	(%edx), %ebx
	movl	4(%edx), %esi
	movl	8(%edx), %edi

#if defined(__ELF__) && defined(__linux__)
	.section .note.GNU-stack,"",%progbits
#endif
