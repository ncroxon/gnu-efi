#ifndef _EFI_LEGACY_EFILIB_H
#define _EFI_LEGACY_EFILIB_H

/*
ABIs:
Default ABI will be highest number
Older versions can be selected via compiler defines
*/

/* Option to set all compat to GNU-EFI 3.0
   To keep existing apps working without change
   Default to current ABI
*/
#ifdef GNU_EFI_3_0_COMPAT
#define GNU_EFI_USE_REALLOCATEPOOL_ABI 0
#define GNU_EFI_USE_COPYMEM_ABI 0
#define GNU_EFI_USE_COMPAREGUID_ABI 0
#endif

/* ReallocatePool */
#ifndef GNU_EFI_USE_REALLOCATEPOOL_ABI
#define GNU_EFI_USE_REALLOCATEPOOL_ABI 1
#endif

#if GNU_EFI_USE_REALLOCATEPOOL_ABI == 0
#define ReallocatePool(a,b,c) ReallocatePool_0(a,b,c)
#else
#define ReallocatePool(a,b,c) ReallocatePool_1(a,b,c)
#endif

/* prevent circular headers */
VOID *
EFIAPI
ReallocatePool_1 (
        IN UINTN  OldSize,
        IN UINTN  NewSize,
        IN VOID   *OldPool  OPTIONAL
);

EFI_INTERNAL
VOID *
EFIAPI
ReallocatePool_0 (
    IN VOID                 *OldPool,
    IN UINTN                OldSize,
    IN UINTN                NewSize
)
{
    return ReallocatePool_1(OldSize, NewSize, OldPool);
}

/* end ReallocatePool */

/* CopyMem */
/* CopyMemC isn't deprecated - serves different purpose to CopyMem */

#ifndef GNU_EFI_USE_COPYMEM_ABI
#define GNU_EFI_USE_COPYMEM_ABI 1
#endif

#if GNU_EFI_USE_COPYMEM_ABI == 0
#define CopyMem(a,b,c) CopyMemC(a,b,c)
#else
#define CopyMem(a,b,c) CopyMem_1(a,b,c)
#endif

/* end CopyMem */

/* CompareGuid */

#ifndef GNU_EFI_USE_COMPAREGUID_ABI
#define GNU_EFI_USE_COMPAREGUID_ABI 1
#endif

#if GNU_EFI_USE_COMPAREGUID_ABI == 0
#define CompareGuid(a,b) CompareGuid_0(a,b)
#else
#define CompareGuid(a,b) CompareGuid_1(a,b)
#endif

/* prevent circular headers */
BOOLEAN
EFIAPI
CompareGuid_1 (
    IN CONST EFI_GUID     *Guid1,
    IN CONST EFI_GUID     *Guid2
);

EFI_INTERNAL
INTN
EFIAPI
CompareGuid_0 (
    IN EFI_GUID     *Guid1,
    IN EFI_GUID     *Guid2)
{
    if (CompareGuid_1(Guid1, Guid2)) {
        return 0;
    } else {
        return 1;
    }
}

#endif
