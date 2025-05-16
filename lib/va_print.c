
#include "lib.h"
#include "internal/print.h"
#include "internal/va_print.h"

VOID
CALL_CONV
FUNCTION_NAME(_PoolCatPrint) (
    IN CONST CHAR16     *fmt,
    IN VA_LIST          args,
    IN OUT POOL_PRINT   *spc,
    IN INTN             (EFIAPI *Output)(VOID *context, CHAR16 *str)
    )
// Dispatch function for UnicodeSPrint, PoolPrint, and CatPrint
{
    FUNCTION_NAME(PRINT_STATE)         ps;

    ZeroMem (&ps, sizeof(ps));
    ps.Output  = Output;
    ps.Context = spc;
    ps.fmt.pw = fmt;
    VA_COPY(ps.args, args);
    FUNCTION_NAME(_Print) (&ps);
    VA_END(ps.args);
}

UINTN
CALL_CONV
FUNCTION_NAME(UnicodeVSPrint) (
    OUT CHAR16        *Str,
    IN UINTN          StrSize,
    IN CONST CHAR16   *fmt,
    VA_LIST           args
    )
/*++

Routine Description:

    Prints a formatted unicode string to a buffer using a VA_LIST

Arguments:

    Str         - Output buffer to print the formatted string into

    StrSize     - Size of Str (in bytes).  String is truncated to this size.
                  A size of 0 means there is no limit

    fmt         - The format string

    args        - VA_LIST


Returns:

    String length returned in buffer

--*/
{
    POOL_PRINT          spc;

    spc.str    = Str;
    spc.maxlen = StrSize / sizeof(CHAR16) - 1;
    spc.len    = 0;

    FUNCTION_NAME(_PoolCatPrint) (fmt, args, &spc, _SPrint);

    return spc.len;
}

CHAR16 *
CALL_CONV
FUNCTION_NAME(VPoolPrint) (
    IN CONST CHAR16     *fmt,
    VA_LIST             args
    )
/*++

Routine Description:

    Prints a formatted unicode string to allocated pool using VA_LIST argument.
    The caller must free the resulting buffer.

Arguments:

    fmt         - The format string
    args        - The arguments in VA_LIST form

Returns:

    Allocated buffer with the formatted string printed in it.
    The caller must free the allocated buffer.   The buffer
    allocation is not packed.

--*/
{
    POOL_PRINT          spc;
    ZeroMem (&spc, sizeof(spc));
    FUNCTION_NAME(_PoolCatPrint) (fmt, args, &spc, _PoolPrint);
    return spc.str;
}

UINTN
CALL_CONV
FUNCTION_NAME(VPrint) (
    IN CONST CHAR16   *fmt,
    VA_LIST           args
    )
/*++

Routine Description:

    Prints a formatted unicode string to the default console using a VA_LIST

Arguments:

    fmt         - Format string
    args        - VA_LIST
Returns:

    Length of string printed to the console

--*/
{
    return FUNCTION_NAME(_IPrint) ((UINTN) -1, (UINTN) -1, ST->ConOut, fmt, NULL, args);
}

UINTN
CALL_CONV
FUNCTION_NAME(_IPrint) (
    IN UINTN                            Column,
    IN UINTN                            Row,
    IN SIMPLE_TEXT_OUTPUT_INTERFACE     *Out,
    IN CONST CHAR16                     *fmt,
    IN CONST CHAR8                      *fmta,
    IN VA_LIST                          args
    )
// Display string worker for: Print, PrintAt, IPrint, IPrintAt
{
    FUNCTION_NAME(PRINT_STATE)     ps;
    UINTN            back;

    ZeroMem (&ps, sizeof(ps));
    ps.Context = Out;
    ps.Output  = (INTN (EFIAPI *)(VOID *, CHAR16 *)) Out->OutputString;
    ps.SetAttr = (INTN (EFIAPI *)(VOID *, UINTN))  Out->SetAttribute;
    ps.Attr = Out->Mode->Attribute;

    back = (ps.Attr >> 4) & 0xF;
    ps.AttrNorm = EFI_TEXT_ATTR(EFI_LIGHTGRAY, back);
    ps.AttrHighlight = EFI_TEXT_ATTR(EFI_WHITE, back);
    ps.AttrError = EFI_TEXT_ATTR(EFI_YELLOW, back);

    if (fmt) {
        ps.fmt.pw = fmt;
    } else {
        ps.fmt.Ascii = TRUE;
        ps.fmt.pc = fmta;
    }

    VA_COPY(ps.args, args);

    if (Column != (UINTN) -1) {
        uefi_call_wrapper(Out->SetCursorPosition, 3, Out, Column, Row);
    }

    back = FUNCTION_NAME(_Print) (&ps);
    VA_END(ps.args);
    return back;
}

UINTN
CALL_CONV
FUNCTION_NAME(AsciiVSPrint) (
    OUT CHAR8         *Str,
    IN UINTN          StrSize,
    IN CONST CHAR8    *fmt,
    VA_LIST           args
)
/*++

Routine Description:

    Prints a formatted ascii string to a buffer using a VA_LIST

Arguments:

    Str         - Output buffer to print the formatted string into

    StrSize     - Size of Str (in bytes).  String is truncated to this size.
                  A size of 0 means there is no limit

    fmt         - The format string

    args        - VA_LIST


Returns:

    String length returned in buffer

--*/
// Use FUNCTION_NAME(UnicodeVSPrint)() and convert back to ASCII
{
    CHAR16 *UnicodeStr, *UnicodeFmt;
    UINTN i, Len;

    UnicodeStr = AllocatePool(StrSize * sizeof(CHAR16));
    if (!UnicodeStr)
        return 0;

    UnicodeFmt = PoolPrint(u"%a", fmt);
    if (!UnicodeFmt) {
        FreePool(UnicodeStr);
        return 0;
    }

    Len = FUNCTION_NAME(UnicodeVSPrint)(UnicodeStr, StrSize * sizeof(CHAR16), UnicodeFmt, args);
    FreePool(UnicodeFmt);

    // The strings are ASCII so just do a plain Unicode conversion
    for (i = 0; i < Len; i++)
        Str[i] = (CHAR8)UnicodeStr[i];
    Str[Len] = 0;
    FreePool(UnicodeStr);

    return Len;
}
