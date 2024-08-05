#ifndef _EFI_LEGACY_H
#define _EFI_LEGACY_H

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
#define ReallocatePool ReallocatePool_0
#else
#define ReallocatePool ReallocatePool_1
#endif

/* prevent circular headers */
VOID *
EFIAPI
ReallocatePool_1 (
        IN UINTN  OldSize,
        IN UINTN  NewSize,
        IN VOID   *OldPool  OPTIONAL
);

STATIC
inline
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
#define CopyMem CopyMemC
#else
#define CopyMem CopyMem_1
#endif

/* end CopyMem */

/* CompareGuid */

#ifndef GNU_EFI_USE_COMPAREGUID_ABI
#define GNU_EFI_USE_COMPAREGUID_ABI 1
#endif

#if GNU_EFI_USE_COMPAREGUID_ABI == 0
#define CompareGuid CompareGuid_0
#else
#define CompareGuid CompareGuid_1
#endif

/* prevent circular headers */
BOOLEAN
EFIAPI
CompareGuid_1 (
    IN CONST EFI_GUID     *Guid1,
    IN CONST EFI_GUID     *Guid2
);

STATIC
inline
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
