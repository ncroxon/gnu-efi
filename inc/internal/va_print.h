
#if !defined(__GNU_EFI_INTERNAL_VA_PRINT)
#define __GNU_EFI_INTERNAL_VA_PRINT

#include <efistdarg.h>
#include "print.h"

#if defined(__GNU_EFI_INTERNAL_USE_MS_VA_ABI)
typedef ms_va_list VA_LIST;
#define VA_START(a,b) ms_va_start(a,b)
#define VA_COPY(a,b) ms_va_copy(a,b)
#define VA_END(a) ms_va_end(a)
#define VA_ARG(a,b) ms_va_arg(a,b)
#else
typedef va_list VA_LIST;
#define VA_START(a,b) va_start(a,b)
#define VA_COPY(a,b) va_copy(a,b)
#define VA_END(a) va_end(a)
#define VA_ARG(a,b) va_arg(a,b)
#endif

#if defined(__GNU_EFI_INTERNAL_USE_MS_VA_ABI)
#define FUNCTION_NAME(a) MS_ ## a
#define CALL_CONV EFIAPI
#else
#define FUNCTION_NAME(a) a
#define CALL_CONV
#endif

//
// Declare runtime functions
//

#ifdef RUNTIME_CODE
#ifndef __GNUC__

// For debugging..

/*
#pragma RUNTIME_CODE(FUNCTION_NAME(_Print))
#pragma RUNTIME_CODE(FUNCTION_NAME(PFLUSH))
#pragma RUNTIME_CODE(FUNCTION_NAME(PSETATTR))
#pragma RUNTIME_CODE(FUNCTION_NAME(PPUTC))
#pragma RUNTIME_CODE(FUNCTION_NAME(PITEM))
*/

#endif /* !defined(__GNUC__) */
#endif

typedef struct FUNCTION_NAME(_pstate) {
    // Input
    POINTER     fmt;
    VA_LIST     args;

    // Output
    CHAR16      *Buffer;
    CHAR16      *End;
    CHAR16      *Pos;
    UINTN       Len;

    UINTN       Attr;
    UINTN       RestoreAttr;

    UINTN       AttrNorm;
    UINTN       AttrHighlight;
    UINTN       AttrError;

    INTN        (EFIAPI *Output)(VOID *context, CHAR16 *str);
    INTN        (EFIAPI *SetAttr)(VOID *context, UINTN attr);
    VOID        *Context;

    // Current item being formatted
    struct _pitem  *Item;
} FUNCTION_NAME(PRINT_STATE);

EFI_INTERNAL
UINTN
CALL_CONV
FUNCTION_NAME(_Print) (
    IN FUNCTION_NAME(PRINT_STATE)     *ps
    );

UINTN
CALL_CONV
FUNCTION_NAME(_IPrint) (
    IN UINTN                            Column,
    IN UINTN                            Row,
    IN SIMPLE_TEXT_OUTPUT_INTERFACE     *Out,
    IN CONST CHAR16                     *fmt,
    IN CONST CHAR8                      *fmta,
    IN VA_LIST                          args
    );

EFI_INTERNAL
VOID
CALL_CONV
FUNCTION_NAME(PFLUSH) (
    IN OUT FUNCTION_NAME(PRINT_STATE)     *ps
    );

EFI_INTERNAL
VOID
CALL_CONV
FUNCTION_NAME(PPUTC) (
    IN OUT FUNCTION_NAME(PRINT_STATE)     *ps,
    IN CHAR16              c
    );


EFI_INTERNAL
VOID
CALL_CONV
FUNCTION_NAME(PITEM) (
    IN OUT FUNCTION_NAME(PRINT_STATE)  *ps
    );

EFI_INTERNAL
VOID
CALL_CONV
FUNCTION_NAME(PSETATTR) (
    IN OUT FUNCTION_NAME(PRINT_STATE)  *ps,
    IN UINTN             Attr
    );

EFI_INTERNAL
VOID
CALL_CONV
FUNCTION_NAME(PFLUSH) (
    IN OUT FUNCTION_NAME(PRINT_STATE)     *ps
    )
{
    *ps->Pos = 0;
    if (IsLocalPrint(ps->Output))
	ps->Output(ps->Context, ps->Buffer);
    else
    	uefi_call_wrapper(ps->Output, 2, ps->Context, ps->Buffer);
    ps->Pos = ps->Buffer;
}

EFI_INTERNAL
VOID
CALL_CONV
FUNCTION_NAME(PSETATTR) (
    IN OUT FUNCTION_NAME(PRINT_STATE)  *ps,
    IN UINTN             Attr
    )
{
   FUNCTION_NAME(PFLUSH) (ps);

   ps->RestoreAttr = ps->Attr;
   if (ps->SetAttr) {
	uefi_call_wrapper(ps->SetAttr, 2, ps->Context, Attr);
   }

   ps->Attr = Attr;
}

EFI_INTERNAL
VOID
CALL_CONV
FUNCTION_NAME(PPUTC) (
    IN OUT FUNCTION_NAME(PRINT_STATE)     *ps,
    IN CHAR16              c
    )
{
    // if this is a newline, add a carriage return
    CHAR16 last_char = (ps->Len > 0) ? *(ps->Pos - 1) : '\0';
    if (c == '\n' && last_char != '\r') {
        FUNCTION_NAME(PPUTC) (ps, '\r');
    }

    *ps->Pos = c;
    ps->Pos += 1;
    ps->Len += 1;

    // if at the end of the buffer, flush it
    if (ps->Pos >= ps->End) {
        FUNCTION_NAME(PFLUSH)(ps);
    }
}

EFI_INTERNAL
VOID
CALL_CONV
FUNCTION_NAME(PITEM) (
    IN OUT FUNCTION_NAME(PRINT_STATE)  *ps
    )
{
    UINTN               Len, i;
    PRINT_ITEM          *Item;
    CHAR16              c;

    // Get the length of the item
    Item = ps->Item;
    Item->Item.Index = 0;
    while (Item->Item.Index < Item->FieldWidth) {
        c = PGETC(&Item->Item);
        if (!c) {
            Item->Item.Index -= 1;
            break;
        }
    }
    Len = Item->Item.Index;

    // if there is no item field width, use the items width
    if (Item->FieldWidth == (UINTN) -1) {
        Item->FieldWidth = Len;
    }

    // if item is larger then width, update width
    if (Len > Item->Width) {
        Item->Width = Len;
    }


    // if pad field before, add pad char
    if (Item->PadBefore) {
        for (i=Item->Width; i < Item->FieldWidth; i+=1) {
            FUNCTION_NAME(PPUTC) (ps, ' ');
        }
    }

    // pad item
    for (i=Len; i < Item->Width; i++) {
        FUNCTION_NAME(PPUTC) (ps, Item->Pad);
    }

    // add the item
    Item->Item.Index=0;
    while (Item->Item.Index < Len) {
        FUNCTION_NAME(PPUTC) (ps, PGETC(&Item->Item));
    }

    // If pad at the end, add pad char
    if (!Item->PadBefore) {
        for (i=Item->Width; i < Item->FieldWidth; i+=1) {
            FUNCTION_NAME(PPUTC) (ps, ' ');
        }
    }
}

EFI_INTERNAL
UINTN
CALL_CONV
FUNCTION_NAME(_Print) (
    IN FUNCTION_NAME(PRINT_STATE)     *ps
    )
/*++

Routine Description:

    %w.lF   -   w = width
                l = field width
                F = format of arg

  Args F:
    0       -   pad with zeros
    -       -   justify on left (default is on right)
    ,       -   add comma's to field
    *       -   width provided on stack
    n       -   Set output attribute to normal (for this field only)
    h       -   Set output attribute to highlight (for this field only)
    e       -   Set output attribute to error (for this field only)
    l       -   Value is 64 bits

    a       -   ascii string
    s       -   unicode string
    X       -   fixed 8 byte value in hex
    x       -   hex value
    d       -   value as signed decimal
    u       -   value as unsigned decimal
    f       -   value as floating point
    c       -   Unicode char
    t       -   EFI time structure
    g       -   Pointer to GUID
    r       -   EFI status code (result code)
    D       -   pointer to Device Path with normal ending.

    N       -   Set output attribute to normal
    H       -   Set output attribute to highlight
    E       -   Set output attribute to error
    %       -   Print a %

Arguments:

    SystemTable     - The system table

Returns:

    Number of charactors written

--*/
{
    CHAR16          c;
    UINTN           Attr;
    PRINT_ITEM      Item;
    CHAR16          Buffer[PRINT_STRING_LEN];

    ps->Len = 0;
    ps->Buffer = Buffer;
    ps->Pos = Buffer;
    ps->End = Buffer + PRINT_STRING_LEN - 1;
    ps->Item = &Item;

    ps->fmt.Index = 0;
    while ((c = PGETC(&ps->fmt))) {

        if (c != '%') {
            FUNCTION_NAME(PPUTC) ( ps, c );
            continue;
        }

        // setup for new item
        Item.FieldWidth = (UINTN) -1;
        Item.Width = 0;
        Item.WidthParse = &Item.Width;
        Item.Pad = ' ';
        Item.PadBefore = TRUE;
        Item.Comma = FALSE;
        Item.Long = FALSE;
        Item.Item.Ascii = FALSE;
        Item.Item.pw = NULL;
        ps->RestoreAttr = 0;
        Attr = 0;

        while ((c = PGETC(&ps->fmt))) {

            switch (c) {

            case '%':
                //
                // %% -> %
                //
                Item.Scratch[0] = '%';
                Item.Scratch[1] = 0;
                Item.Item.pw = Item.Scratch;
                break;

            case ',':
                Item.Comma = TRUE;
                break;

            case '-':
                Item.PadBefore = FALSE;
                break;

            case '*':
                *Item.WidthParse = VA_ARG(ps->args, UINTN);
                break;

            case '.':
                Item.WidthParse = &Item.FieldWidth;
                break;

            case '0':
                Item.Pad = '0';
                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                *Item.WidthParse = 0;
                do {
                    *Item.WidthParse = *Item.WidthParse * 10 + c - '0';
                    c = PGETC(&ps->fmt);
                } while (c >= '0'  &&  c <= '9') ;
                ps->fmt.Index -= 1;
                break;

            case 'a':
                Item.Item.pc = VA_ARG(ps->args, CHAR8 *);
                Item.Item.Ascii = TRUE;
                if (!Item.Item.pc) {
                    Item.Item.pc = (CHAR8 *)"(null)";
                }
                break;

            case 'c':
                Item.Scratch[0] = (CHAR16) VA_ARG(ps->args, UINTN);
                Item.Scratch[1] = 0;
                Item.Item.pw = Item.Scratch;
                break;

            case 'D':
            {
                EFI_DEVICE_PATH *dp = VA_ARG(ps->args, EFI_DEVICE_PATH *);
                CHAR16 *dpstr = DevicePathToStr(dp);
                StrnCpy(Item.Scratch, dpstr, PRINT_ITEM_BUFFER_LEN);
                Item.Scratch[PRINT_ITEM_BUFFER_LEN-1] = u'\0';
                FreePool(dpstr);

                Item.Item.pw = Item.Scratch;
                break;
            }

            case 'd':
                ValueToString (
                    Item.Scratch,
                    Item.Comma,
                    Item.Long ? VA_ARG(ps->args, INT64) : VA_ARG(ps->args, INT32)
                    );
                Item.Item.pw = Item.Scratch;
                break;

            case 'E':
                Attr = ps->AttrError;
                break;

            case 'e':
                FUNCTION_NAME(PSETATTR)(ps, ps->AttrError);
                break;

            case 'f':
                FloatToString (
                    Item.Scratch,
                    Item.Comma,
                    VA_ARG(ps->args, double)
                    );
                Item.Item.pw = Item.Scratch;
                break;

            case 'g':
                GuidToString (Item.Scratch, VA_ARG(ps->args, EFI_GUID *));
                Item.Item.pw = Item.Scratch;
                break;

            case 'H':
                Attr = ps->AttrHighlight;
                break;

            case 'h':
                FUNCTION_NAME(PSETATTR)(ps, ps->AttrHighlight);
                break;

            case 'l':
                Item.Long = TRUE;
                break;

            case 'N':
                Attr = ps->AttrNorm;
                break;

            case 'n':
                FUNCTION_NAME(PSETATTR)(ps, ps->AttrNorm);
                break;

            case 'p':
                Item.Width = sizeof(void *) == (8 ? 16 : 8) + 2;
                Item.Pad = '0';
                Item.Scratch[0] = ' ';
                Item.Scratch[1] = ' ';
                ValueToHex (
                    Item.Scratch+2,
                    Item.Long ? VA_ARG(ps->args, UINT64) : VA_ARG(ps->args, UINT32)
                    );
                Item.Scratch[0] = '0';
                Item.Scratch[1] = 'x';
                Item.Item.pw = Item.Scratch;
                break;

            case 'r':
                StatusToString (Item.Scratch, VA_ARG(ps->args, EFI_STATUS));
                Item.Item.pw = Item.Scratch;
                break;

            case 's':
                Item.Item.pw = VA_ARG(ps->args, CHAR16 *);
                if (!Item.Item.pw) {
                    Item.Item.pw = u"(null)";
                }
                break;

            case 't':
                TimeToString (Item.Scratch, VA_ARG(ps->args, EFI_TIME *));
                Item.Item.pw = Item.Scratch;
                break;

            case 'u':
                ValueToString (
                    Item.Scratch,
                    Item.Comma,
                    Item.Long ? VA_ARG(ps->args, UINT64) : VA_ARG(ps->args, UINT32)
                    );
                Item.Item.pw = Item.Scratch;
                break;

            case 'X':
                Item.Width = Item.Long ? 16 : 8;
                Item.Pad = '0';
#if __GNUC__ >= 7
		__attribute__ ((fallthrough));
#endif
            case 'x':
                ValueToHex (
                    Item.Scratch,
                    Item.Long ? VA_ARG(ps->args, UINT64) : VA_ARG(ps->args, UINT32)
                    );
                Item.Item.pw = Item.Scratch;
                break;

            default:
                Item.Scratch[0] = '?';
                Item.Scratch[1] = 0;
                Item.Item.pw = Item.Scratch;
                break;
            }

            // if we have an Item
            if (Item.Item.pw) {
                FUNCTION_NAME(PITEM) (ps);
                break;
            }

            // if we have an Attr set
            if (Attr) {
                FUNCTION_NAME(PSETATTR)(ps, Attr);
                ps->RestoreAttr = 0;
                break;
            }
        }

        if (ps->RestoreAttr) {
            FUNCTION_NAME(PSETATTR)(ps, ps->RestoreAttr);
        }
    }

    // Flush buffer
    FUNCTION_NAME(PFLUSH) (ps);
    return ps->Len;
}

VOID
CALL_CONV
FUNCTION_NAME(_PoolCatPrint) (
    IN CONST CHAR16     *fmt,
    IN VA_LIST          args,
    IN OUT POOL_PRINT   *spc,
    IN INTN             (EFIAPI *Output)(VOID *context, CHAR16 *str)
    );

#endif
