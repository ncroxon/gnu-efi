#ifndef GNU_EFI_SETJMP_H
#define GNU_EFI_SETJMP_H

#if !defined(_MSC_VER)

#include "eficompiler.h"
#include "efisetjmp_arch.h"

extern UINTN base_setjmp(jmp_buf env) __attribute__((returns_twice));
extern VOID base_longjmp(jmp_buf env, UINTN value) EFI_NORETURN;

#define setjmp(env) (int)base_setjmp(env)
#define longjmp(env, value) base_longjmp(env, (UINTN)value)

#endif /* _MSC_VER */

#endif /* GNU_EFI_SETJMP_H */

