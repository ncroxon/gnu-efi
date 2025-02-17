#if !defined(__GNU_EFI_INTERNAL_PRINT)
#define __GNU_EFI_INTERNAL_PRINT

//
// Internal fucntions
//

//
// Declare runtime functions
//

#ifdef RUNTIME_CODE
#ifndef __GNUC__

// For debugging..

/*
#pragma RUNTIME_CODE(PGETC)
*/

#endif /* !defined(__GNUC__) */
#endif

typedef struct {
    BOOLEAN             Ascii;
    UINTN               Index;
    union {
        CONST CHAR16    *pw;
        CONST CHAR8     *pc;
    } un;
} POINTER;

EFI_INTERNAL
CHAR16
PGETC (
    IN POINTER      *p
    );

#define PRINT_STRING_LEN            200
#define PRINT_ITEM_BUFFER_LEN       100

#define pw	un.pw
#define pc	un.pc

typedef struct _pitem {

    POINTER     Item;
    CHAR16      Scratch[PRINT_ITEM_BUFFER_LEN];
    UINTN       Width;
    UINTN       FieldWidth;
    UINTN       *WidthParse;
    CHAR16      Pad;
    BOOLEAN     PadBefore;
    BOOLEAN     Comma;
    BOOLEAN     Long;
} PRINT_ITEM;
    
EFI_INTERNAL
CHAR16
PGETC (
    IN POINTER      *p
    )
{
    CHAR16      c;

    c = p->Ascii ? p->pc[p->Index] : p->pw[p->Index];
    p->Index += 1;

    return  c;
}

INTN EFIAPI
_SPrint (
    IN VOID     *Context,
    IN CHAR16   *Buffer
    );

INTN EFIAPI
_PoolPrint (
    IN VOID     *Context,
    IN CHAR16   *Buffer
    );

INTN EFIAPI
_DbgOut (
    IN VOID     *Context,
    IN CHAR16   *Buffer
    );

EFI_INTERNAL
INTN
IsLocalPrint(void *func)
{
	if (func == _DbgOut || func == _SPrint || func == _PoolPrint)
		return 1;
	return 0;
}

#endif



//
//
//
