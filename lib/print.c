/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    print.c

Abstract:




Revision History

--*/

#include "lib.h"
#include "internal/print.h"
#include "internal/va_print.h"


/* VA types/functions can be used if used internally so we can use default va_list always here */


//
// Declare runtime functions
//

#ifdef RUNTIME_CODE
#ifndef __GNUC__
#pragma RUNTIME_CODE(DbgPrint)

// For debugging..

/*
#pragma RUNTIME_CODE(ValueToHex)
#pragma RUNTIME_CODE(ValueToString)
#pragma RUNTIME_CODE(TimeToString)
*/

#endif /* !defined(__GNUC__) */
#endif

//
//
//

INTN
DbgPrint (
    IN INTN         mask,
    IN CONST CHAR8  *fmt,
    ...
    )
/*++

Routine Description:

    Prints a formatted unicode string to the default StandardError console

Arguments:

    mask        - Bit mask of debug string.  If a bit is set in the
                  mask that is also set in EFIDebug the string is
                  printed; otherwise, the string is not printed

    fmt         - Format string

Returns:

    Length of string printed to the StandardError console

--*/
{
    SIMPLE_TEXT_OUTPUT_INTERFACE    *DbgOut;
    PRINT_STATE     ps;
    va_list         args;
    UINTN           back;
    UINTN           attr;
    UINTN           SavedAttribute;


    if (!(EFIDebug & mask)) {
        return 0;
    }

    va_start (args, fmt);
    ZeroMem (&ps, sizeof(ps));

    ps.Output = _DbgOut;
    ps.fmt.Ascii = TRUE;
    ps.fmt.pc = fmt;
    va_copy(ps.args, args);
    ps.Attr = EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_RED);

    DbgOut = LibRuntimeDebugOut;

    if (!DbgOut) {
        DbgOut = ST->StdErr;
    }

    if (DbgOut) {
        ps.Attr = DbgOut->Mode->Attribute;
        ps.Context = DbgOut;
        ps.SetAttr = (INTN (EFIAPI *)(VOID *, UINTN))  DbgOut->SetAttribute;
    }

    SavedAttribute = ps.Attr;

    back = (ps.Attr >> 4) & 0xf;
    ps.AttrNorm = EFI_TEXT_ATTR(EFI_LIGHTGRAY, back);
    ps.AttrHighlight = EFI_TEXT_ATTR(EFI_WHITE, back);
    ps.AttrError = EFI_TEXT_ATTR(EFI_YELLOW, back);

    attr = ps.AttrNorm;

    if (mask & D_WARN) {
        attr = ps.AttrHighlight;
    }

    if (mask & D_ERROR) {
        attr = ps.AttrError;
    }

    if (ps.SetAttr) {
        ps.Attr = attr;
        uefi_call_wrapper(ps.SetAttr, 2, ps.Context, attr);
    }

    _Print (&ps);

    va_end (ps.args);
    va_end (args);

    //
    // Restore original attributes
    //

    if (ps.SetAttr) {
        uefi_call_wrapper(ps.SetAttr, 2, ps.Context, SavedAttribute);
    }

    return 0;
}

INTN EFIAPI
_DbgOut (
    IN VOID     *Context,
    IN CHAR16   *Buffer
    )
// Append string worker for DbgPrint
{
    SIMPLE_TEXT_OUTPUT_INTERFACE    *DbgOut;

    DbgOut = Context;
//    if (!DbgOut && ST && ST->ConOut) {
//        DbgOut = ST->ConOut;
//    }

    if (DbgOut) {
	if (IsLocalPrint(DbgOut->OutputString))
		DbgOut->OutputString(DbgOut, Buffer);
        else
		uefi_call_wrapper(DbgOut->OutputString, 2, DbgOut, Buffer);
    }

    return 0;
}

INTN EFIAPI
_SPrint (
    IN VOID     *Context,
    IN CHAR16   *Buffer
    )
// Append string worker for UnicodeSPrint, PoolPrint and CatPrint
{
    UINTN           len;
    POOL_PRINT      *spc;

    spc = Context;
    len = StrLen(Buffer);

    //
    // Is the string is over the max truncate it
    //

    if (spc->len + len > spc->maxlen) {
        len = spc->maxlen - spc->len;
    }

    //
    // Append the new text
    //

    CopyMem (spc->str + spc->len, Buffer, len * sizeof(CHAR16));
    spc->len += len;

    //
    // Null terminate it
    //

    if (spc->len < spc->maxlen) {
        spc->str[spc->len] = 0;
    } else if (spc->maxlen) {
        spc->str[spc->maxlen] = 0;
    }

    return 0;
}


INTN EFIAPI
_PoolPrint (
    IN VOID     *Context,
    IN CHAR16   *Buffer
    )
// Append string worker for PoolPrint and CatPrint
{
    UINTN           newlen;
    POOL_PRINT      *spc;

    spc = Context;
    newlen = spc->len + StrLen(Buffer) + 1;

    //
    // Is the string is over the max, grow the buffer
    //

    if (newlen > spc->maxlen) {

        //
        // Grow the pool buffer
        //

        newlen += PRINT_STRING_LEN;
        spc->maxlen = newlen;
        spc->str = ReallocatePool (
                        spc->len * sizeof(CHAR16),
                        spc->maxlen * sizeof(CHAR16),
                        spc->str
                        );

        if (!spc->str) {
            spc->len = 0;
            spc->maxlen = 0;
        }
    }

    //
    // Append the new text
    //

    return _SPrint (Context, Buffer);
}

UINTN
UnicodeSPrint (
    OUT CHAR16        *Str,
    IN UINTN          StrSize,
    IN CONST CHAR16   *fmt,
    ...
    )
/*++

Routine Description:

    Prints a formatted unicode string to a buffer

Arguments:

    Str         - Output buffer to print the formatted string into

    StrSize     - Size of Str.  String is truncated to this size.
                  A size of 0 means there is no limit

    fmt         - The format string

Returns:

    String length returned in buffer

--*/
{
    va_list          args;
    UINTN            len;

    va_start (args, fmt);
    len = UnicodeVSPrint(Str, StrSize, fmt, args);
    va_end (args);

    return len;
}

UINTN
AsciiSPrint (
    OUT CHAR8         *Str,
    IN UINTN          StrSize,
    IN CONST CHAR8    *fmt,
    ...
    )
/*++

Routine Description:

    Prints a formatted unicode string to a buffer

Arguments:

    Str         - Output buffer to print the formatted string into

    StrSize     - Size of Str.  String is truncated to this size.
                  A size of 0 means there is no limit

    fmt         - The format string

Returns:

    String length returned in buffer

--*/
{
    va_list       args;
    UINTN         len;

    va_start (args, fmt);
    len = AsciiVSPrint(Str, StrSize, fmt, args);
    va_end (args);

    return len;
}

CHAR16 *
PoolPrint (
    IN CONST CHAR16     *fmt,
    ...
    )
/*++

Routine Description:

    Prints a formatted unicode string to allocated pool.  The caller
    must free the resulting buffer.

Arguments:

    fmt         - The format string

Returns:

    Allocated buffer with the formatted string printed in it.
    The caller must free the allocated buffer.   The buffer
    allocation is not packed.

--*/
{
    va_list args;
    CHAR16 *pool;
    va_start (args, fmt);
    pool = VPoolPrint(fmt, args);
    va_end (args);
    return pool;
}

CHAR16 *
CatPrint (
    IN OUT POOL_PRINT   *Str,
    IN CONST CHAR16     *fmt,
    ...
    )
/*++

Routine Description:

    Concatenates a formatted unicode string to allocated pool.
    The caller must free the resulting buffer.

Arguments:

    Str         - Tracks the allocated pool, size in use, and
                  amount of pool allocated.

    fmt         - The format string

Returns:

    Allocated buffer with the formatted string printed in it.
    The caller must free the allocated buffer.   The buffer
    allocation is not packed.

--*/
{
    va_list             args;

    va_start (args, fmt);
    _PoolCatPrint (fmt, args, Str, _PoolPrint);
    va_end (args);
    return Str->str;
}



UINTN
Print (
    IN CONST CHAR16   *fmt,
    ...
    )
/*++

Routine Description:

    Prints a formatted unicode string to the default console

Arguments:

    fmt         - Format string

Returns:

    Length of string printed to the console

--*/
{
    va_list     args;
    UINTN       back;

    va_start (args, fmt);
    back = _IPrint ((UINTN) -1, (UINTN) -1, ST->ConOut, fmt, NULL, args);
    va_end (args);
    return back;
}


UINTN
PrintAt (
    IN UINTN          Column,
    IN UINTN          Row,
    IN CONST CHAR16   *fmt,
    ...
    )
/*++

Routine Description:

    Prints a formatted unicode string to the default console, at
    the supplied cursor position

Arguments:

    Column, Row - The cursor position to print the string at

    fmt         - Format string

Returns:

    Length of string printed to the console

--*/
{
    va_list     args;
    UINTN       back;

    va_start (args, fmt);
    back = _IPrint (Column, Row, ST->ConOut, fmt, NULL, args);
    va_end (args);
    return back;
}


UINTN
IPrint (
    IN SIMPLE_TEXT_OUTPUT_INTERFACE    *Out,
    IN CONST CHAR16                    *fmt,
    ...
    )
/*++

Routine Description:

    Prints a formatted unicode string to the specified console

Arguments:

    Out         - The console to print the string too

    fmt         - Format string

Returns:

    Length of string printed to the console

--*/
{
    va_list     args;
    UINTN       back;

    va_start (args, fmt);
    back = _IPrint ((UINTN) -1, (UINTN) -1, Out, fmt, NULL, args);
    va_end (args);
    return back;
}


UINTN
IPrintAt (
    IN SIMPLE_TEXT_OUTPUT_INTERFACE     *Out,
    IN UINTN                            Column,
    IN UINTN                            Row,
    IN CONST CHAR16                     *fmt,
    ...
    )
/*++

Routine Description:

    Prints a formatted unicode string to the specified console, at
    the supplied cursor position

Arguments:

    Out         - The console to print the string to

    Column, Row - The cursor position to print the string at

    fmt         - Format string

Returns:

    Length of string printed to the console

--*/
{
    va_list     args;
    UINTN       back;

    va_start (args, fmt);
    back = _IPrint (Column, Row, Out, fmt, NULL, args);
    va_end (args);
    return back;
}


UINTN
AsciiPrint (
    IN CONST CHAR8    *fmt,
    ...
    )
/*++

Routine Description:

    For those whom really can't deal with unicode, a print
    function that takes an ascii format string

Arguments:

    fmt         - ascii format string

Returns:

    Length of string printed to the console

--*/

{
    va_list     args;
    UINTN       back;

    va_start (args, fmt);
    back = _IPrint ((UINTN) -1, (UINTN) -1, ST->ConOut, NULL, fmt, args);
    va_end (args);
    return back;
}


STATIC CHAR8 Hex[] = {'0','1','2','3','4','5','6','7',
                      '8','9','A','B','C','D','E','F'};

VOID
ValueToHex (
    IN CHAR16   *Buffer,
    IN UINT64   v
    )
{
    CHAR8           str[30], *p1;
    CHAR16          *p2;

    if (!v) {
        Buffer[0] = '0';
        Buffer[1] = 0;
        return ;
    }

    p1 = str;
    p2 = Buffer;

    while (v) {
        // Without the cast, the MSVC compiler may insert a reference to __allmull
        *(p1++) = Hex[(UINTN)(v & 0xf)];
        v = RShiftU64 (v, 4);
    }

    while (p1 != str) {
        *(p2++) = *(--p1);
    }
    *p2 = 0;
}


VOID
ValueToString (
    IN CHAR16   *Buffer,
    IN BOOLEAN  Comma,
    IN INT64    v
    )
{
    STATIC CHAR8 ca[] = {  3, 1, 2 };
    CHAR8        str[40], *p1;
    CHAR16       *p2;
    UINTN        c, r;

    if (!v) {
        Buffer[0] = '0';
        Buffer[1] = 0;
        return ;
    }

    p1 = str;
    p2 = Buffer;

    if (v < 0) {
        *(p2++) = '-';
        v = -v;
    }

    while (v) {
        v = (INT64)DivU64x32 ((UINT64)v, 10, &r);
        *(p1++) = (CHAR8)r + '0';
    }

    c = (UINTN) (Comma ? ca[(p1 - str) % 3] : 999) + 1;
    while (p1 != str) {

        c -= 1;
        if (!c) {
            *(p2++) = ',';
            c = 3;
        }

        *(p2++) = *(--p1);
    }
    *p2 = 0;
}

// Having this call inlined by VS2022 on Release builds produces an
// "Undefined OpCode Exception" on ARM32 whenever Print() is invoked,
// even when no part of the code below is actually being executed...
// For safety, add an explicit clause to prevent inlining on all platforms.
EFI_NOINLINE
VOID
FloatToString (
    IN CHAR16   *Buffer,
    IN BOOLEAN  Comma,
    IN double   v
    )
{
    /*
     * Integer part.
     */
    INTN i = (INTN)v;
    ValueToString(Buffer, Comma, i);


    /*
     * Decimal point.
     */
    UINTN x = StrLen(Buffer);
    Buffer[x] = u'.';
    x++;


    /*
     * Keep fractional part.
     */
    float f = (float)(v - i);
    if (f < 0) f = -f;


    /*
     * Leading fractional zeroes.
     */
    f *= 10.0;
    while (   (f != 0)
           && ((INTN)f == 0))
    {
      Buffer[x] = u'0';
      x++;
      f *= 10.0;
    }


    /*
     * Fractional digits.
     */
    while ((float)(INTN)f != f)
    {
      f *= 10;
    }
    ValueToString(Buffer + x, FALSE, (INTN)f);
    return;
}

VOID
TimeToString (
    OUT CHAR16      *Buffer,
    IN EFI_TIME     *Time
    )
{
    UINTN       Hour, Year;
    CHAR16      AmPm;

    AmPm = 'a';
    Hour = Time->Hour;
    if (Time->Hour == 0) {
        Hour = 12;
    } else if (Time->Hour >= 12) {
        AmPm = 'p';
        if (Time->Hour >= 13) {
            Hour -= 12;
        }
    }

    Year = Time->Year % 100;

    // bugbug: for now just print it any old way
    UnicodeSPrint (Buffer, 0, u"%02d/%02d/%02d  %02d:%02d%c",
        Time->Month,
        Time->Day,
        Year,
        Hour,
        Time->Minute,
        AmPm
        );
}




VOID
DumpHex (
    IN UINTN        Indent,
    IN UINTN        Offset,
    IN UINTN        DataSize,
    IN VOID         *UserData
    )
{
    CHAR8           *Data, Val[50], Str[20], c;
    UINTN           Size, Index;

    UINTN           ScreenCount;
    UINTN           TempColumn;
    UINTN           ScreenSize;
    CHAR16          ReturnStr[1];


    uefi_call_wrapper(ST->ConOut->QueryMode, 4, ST->ConOut, ST->ConOut->Mode->Mode, &TempColumn, &ScreenSize);
    ScreenCount = 0;
    ScreenSize -= 2;

    Data = UserData;
    while (DataSize) {
        Size = 16;
        if (Size > DataSize) {
            Size = DataSize;
        }

        for (Index=0; Index < Size; Index += 1) {
            c = Data[Index];
            Val[Index*3+0] = Hex[c>>4];
            Val[Index*3+1] = Hex[c&0xF];
            Val[Index*3+2] = (Index == 7)?'-':' ';
            Str[Index] = (c < ' ' || c > 'z') ? '.' : c;
        }

        Val[Index*3] = 0;
        Str[Index] = 0;
        Print (u"%*a%X: %-.48a *%a*\n", Indent, "", Offset, Val, Str);

        Data += Size;
        Offset += Size;
        DataSize -= Size;

        ScreenCount++;
        if (ScreenCount >= ScreenSize && ScreenSize != 0) {
            //
            // If ScreenSize == 0 we have the console redirected so don't
            //  block updates
            //
            ScreenCount = 0;
            Print (u"Press Enter to continue :");
            Input (u"", ReturnStr, sizeof(ReturnStr)/sizeof(CHAR16));
            Print (u"\n");
        }

    }
}
