/*++

Copyright (c) 2016-2024 Pete Batard <pete@akeo.ie>

Module Name:

    eficompiler.h

Abstract:

    Compiler specific adjustments

--*/

#if defined(__GNUC__)
#define EFI_UNUSED __attribute__((__unused__))
#else
#define EFI_UNUSED
#endif

#if defined(__clang__)
#define EFI_NO_TAIL_CALL __attribute__((disable_tail_calls))
#elif defined(__GNUC__)
#define EFI_NO_TAIL_CALL __attribute__((optimize("no-optimize-sibling-calls")))
#else
#define EFI_NO_TAIL_CALL
#endif

#if defined(__clang__)
#define EFI_OPTNONE __attribute__((optnone))
#elif defined(__GNUC__)
#define EFI_OPTNONE __attribute__((__optimize__("0")))
#else
#define EFI_OPTNONE
#endif

#if defined(__GNUC__)
#define EFI_ALIGN(x) __attribute__((__aligned__(x)))
#elif defined(_MSC_VER)
#define EFI_ALIGN(x) __declspec(align(x))
#else
// Not being able to align is likely to break executables, so report it
#error "No alignement directive is defined for this platform"
#endif

#if !defined(ALIGN)
#define ALIGN(x) EFI_ALIGN(x)
#endif

#if defined(__GNUC__)
#define EFI_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define EFI_NORETURN __declspec(noreturn)
#else
#define EFI_NORETURN
#endif

#if defined(__GNUC__)
#define EFI_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define EFI_NOINLINE __declspec(noinline)
#else
#define EFI_NOINLINE
#endif

/* Also add a catch-all on __attribute__() for MS compilers */
#if defined(_MSC_VER)
#define __attribute__(x)
#endif
