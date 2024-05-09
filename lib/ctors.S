/*
 * Try to define the minimal empty init/ctor/dtor/fini_arrays so building with
 * older or out-of-tree linker scripts will still work.
 */
/*
 * Note that these aren't using the GNU "CONSTRUCTOR" output section
 * command, so they don't start with a size.  Because of p2align and the
 * end/END definitions, and the fact that they're mergeable, they can also
 * have NULLs which aren't guaranteed to be at the end.
 */
#if defined(__ELF__)
	.section .init_array,"aw",%init_array
#else
	.section .init_array,"aw"
#endif
	.p2align 4, 0
	.globl __init_array_start
__init_array_start:
	.globl __init_array_end
__init_array_end:
#if defined(__ELF__)
	.section .ctors,"aw",%progbits
#else
	.section .ctors,"aw"
#endif
	.p2align 4, 0
	.globl __CTOR_LIST__
__CTOR_LIST__:
	.globl __CTOR_END__
__CTOR_END__:
#if defined(__ELF__)
	.section .dtors,"aw",%progbits
#else
	.section .dtors,"aw"
#endif
	.p2align 4, 0
	.globl __DTOR_LIST__
__DTOR_LIST__:
	.globl __DTOR_END__
__DTOR_END__:
#if defined(__ELF__)
	.section .fini_array,"aw",%fini_array
#else
	.section .fini_array,"aw"
#endif
	.p2align 4, 0
	.globl __fini_array_start
__fini_array_start:
	.globl __fini_array_end
__fini_array_end:

#if defined(__ELF__) && defined(__linux__)
	.section .note.GNU-stack,"",%progbits
#endif

