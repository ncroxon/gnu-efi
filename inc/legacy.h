#ifndef _EFI_LEGACY_H
#define _EFI_LEGACY_H

/*
ABIs:
Default ABI will be highest number
Older versions can be selected via compiler defines
*/

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

#endif

