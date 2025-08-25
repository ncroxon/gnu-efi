/*++

Copyright (c) 1999  Intel Corporation

Module Name:

    EfiRtLib.h

Abstract:

    EFI Runtime library functions



Revision History

--*/

#include "efi.h"
#include "efilib.h"
#include "efirtlib.h"

#ifndef __GNUC__
#pragma RUNTIME_CODE(RtZeroMem)
#endif
VOID
RUNTIMEFUNCTION
RtZeroMem (
    IN VOID     *Buffer,
    IN UINTN     Size
    )
{
    UINT8        *pt;

    pt = Buffer;
    while (Size--) {
        *(pt++) = 0;
    }
}

#ifndef __GNUC__
#pragma RUNTIME_CODE(RtSetMem)
#endif
VOID
EFIAPI
RUNTIMEFUNCTION
RtSetMem (
    IN VOID     *Buffer,
    IN UINTN    Size,
    IN UINT8    Value
    )
{
    UINT8        *pt;

    pt = Buffer;
    while (Size--) {
        *(pt++) = Value;
    }
}

#ifndef __GNUC__
#pragma RUNTIME_CODE(RtCopyMem)
#endif
VOID
EFIAPI
RUNTIMEFUNCTION
RtCopyMem (
    IN VOID        *Dest,
    IN VOID  *Src,
    IN UINTN       len
    )
{
    UINT8 *d, *s;

    d = Dest;
    s = Src;

    if (d == NULL || s == NULL || s == d)
        return;

    // If the beginning of the destination range overlaps with the end of
    // the source range, make sure to start the copy from the end so that
    // we don't end up overwriting source data that we need for the copy.
    if ((d > s) && (d < s + len)) {
        for (d += len, s += len; len--; )
            *--d = *--s;
    } else {
        while (len--)
            *d++ = *s++;
    }
}

#ifndef __GNUC__
#pragma RUNTIME_CODE(RtCopyMemC)
#endif
VOID
EFIAPI
RUNTIMEFUNCTION
RtCopyMemC (
    IN VOID        *Dest,
    IN CONST VOID  *Src,
    IN UINTN       len
    )
{
    /* CopyMem matches ISO C apart from the change to NON-CONST Src
       Overwriting Src is an intended outcome if overlapping occurs (per memmove)
       This function is useful to avoid GCC dying in changing pointer setup
    */
    RtCopyMem(Dest, (VOID*)Src, len);
}

#ifndef __GNUC__
#pragma RUNTIME_CODE(RtCompareMem)
#endif
INTN
RUNTIMEFUNCTION
RtCompareMem (
    IN CONST VOID     *Dest,
    IN CONST VOID     *Src,
    IN UINTN    len
    )
{
    CONST UINT8 *d, *s;

    d = Dest;
    s = Src;

    while (len--) {
        if (*d != *s) {
            return *d - *s;
        }

        d += 1;
        s += 1;
    }

    return 0;
}


typedef EFI_ALIGN(8) UINT32 QUAD_UINT32[4]; /* EFI_GUID is 128 bits so 32 x 4 */

#ifndef __GNUC__
#pragma RUNTIME_CODE(RtCompareGuid)
#endif
BOOLEAN
EFIAPI
RUNTIMEFUNCTION
RtCompareGuid (
    IN CONST EFI_GUID     *Guid1,
    IN CONST EFI_GUID     *Guid2
    )
/*++

Routine Description:

    Compares two GUIDs

Arguments:

    Guid1       - guid to compare
    Guid2       - guid to compare

Returns:
    = 1     if Guid1 == Guid2

--*/
{
    CONST QUAD_UINT32 *g1, *g2;
    UINT32 r;

    //
    // Compare 32 bits at a time
    //

    g1 = (CONST QUAD_UINT32*)Guid1;
    g2 = (CONST QUAD_UINT32*)Guid2;

    r  = (*g1)[0] - (*g2)[0];
    r |= (*g1)[1] - (*g2)[1];
    r |= (*g1)[2] - (*g2)[2];
    r |= (*g1)[3] - (*g2)[3];

    if (r==0) {
        return 1;
    } else {
        return 0;
    }
}

#ifndef __GNUC__
#pragma RUNTIME_CODE(RtIsZeroGuid)
#endif
BOOLEAN
EFIAPI
RUNTIMEFUNCTION
RtIsZeroGuid (
IN CONST EFI_GUID     *Guid1
)
{
	return RtCompareGuid(Guid1, &gZeroGuid);

}
