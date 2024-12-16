#ifndef _EFISTDARG_H_
#define _EFISTDARG_H_

/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    devpath.h

Abstract:

    Defines for parsing the EFI Device Path structures



Revision History

--*/

/* Use somebody else's definitions */
#if defined(GNU_EFI_USE_EXTERNAL_STDARG)
#include <stdarg.h>
#else

/* MSVC */
#if defined(_MSC_VER)
#include <stdarg.h>
typedef va_list ms_va_list;
#define ms_va_start(v,l) va_start(v,l)
#define ms_va_end(v) va_end(v)
#define ms_va_arg(v,l) va_arg(v,l)
#define ms_va_copy(d,s) va_copy(d,s)
#endif

/* GCC x86_64 */
#if !defined(_MSC_VER) && defined(__x86_64__)
typedef __builtin_va_list va_list;
#define va_start(v,l) __builtin_va_start(v,l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v,l) __builtin_va_arg(v,l)
#define va_copy(d,s) __builtin_va_copy(d,s)
typedef __builtin_ms_va_list ms_va_list;
#define ms_va_start(v,l) __builtin_ms_va_start(v,l)
#define ms_va_end(v) __builtin_ms_va_end(v)
#define ms_va_arg(v,l) __builtin_va_arg(v,l) /* There is no __builtin_ms_va_arg */
#define ms_va_copy(d,s) __builtin_ms_va_copy(d,s)
#endif

/* GCC non-x86_64 */
#if !defined(_MSC_VER) && !defined(__x86_64__)
typedef __builtin_va_list va_list;
#define va_start(v,l) __builtin_va_start(v,l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v,l) __builtin_va_arg(v,l)
#define va_copy(d,s) __builtin_va_copy(d,s)
typedef __builtin_va_list ms_va_list;
#define ms_va_start(v,l) __builtin_va_start(v,l)
#define ms_va_end(v) __builtin_va_end(v)
#define ms_va_arg(v,l) __builtin_va_arg(v,l)
#define ms_va_copy(d,s) __builtin_va_copy(d,s)
#endif



#endif
#endif
