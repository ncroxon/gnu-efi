/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    BoxDraw.c

Abstract:
    Lib functions to support Box Draw Unicode code pages.



Revision History

--*/

#include "lib.h"

typedef struct {
    CHAR16  Unicode;
    CHAR8   PcAnsi;
    CHAR8   Ascii;
} UNICODE_TO_CHAR;


//
// This list is used to define the valid extend chars.
// It also provides a mapping from Unicode to PCANSI or
// ASCII. The ASCII mapping we just made up.
//
//

STATIC UNICODE_TO_CHAR UnicodeToPcAnsiOrAscii[] = {
    { BOXDRAW_HORIZONTAL,                 0xc4, u'-'}, 
    { BOXDRAW_VERTICAL,                   0xb3, u'|'},
    { BOXDRAW_DOWN_RIGHT,                 0xda, u'/'},
    { BOXDRAW_DOWN_LEFT,                  0xbf, u'\\'},
    { BOXDRAW_UP_RIGHT,                   0xc0, u'\\'},
    { BOXDRAW_UP_LEFT,                    0xd9, u'/'},
    { BOXDRAW_VERTICAL_RIGHT,             0xc3, u'|'},
    { BOXDRAW_VERTICAL_LEFT,              0xb4, u'|'},
    { BOXDRAW_DOWN_HORIZONTAL,            0xc2, u'+'},
    { BOXDRAW_UP_HORIZONTAL,              0xc1, u'+'},
    { BOXDRAW_VERTICAL_HORIZONTAL,        0xc5, u'+'},
    { BOXDRAW_DOUBLE_HORIZONTAL,          0xcd, u'-'},
    { BOXDRAW_DOUBLE_VERTICAL,            0xba, u'|'},
    { BOXDRAW_DOWN_RIGHT_DOUBLE,          0xd5, u'/'},
    { BOXDRAW_DOWN_DOUBLE_RIGHT,          0xd6, u'/'},
    { BOXDRAW_DOUBLE_DOWN_RIGHT,          0xc9, u'/'},
    { BOXDRAW_DOWN_LEFT_DOUBLE,           0xb8, u'\\'},
    { BOXDRAW_DOWN_DOUBLE_LEFT,           0xb7, u'\\'},
    { BOXDRAW_DOUBLE_DOWN_LEFT,           0xbb, u'\\'},
    { BOXDRAW_UP_RIGHT_DOUBLE,            0xd4, u'\\'},
    { BOXDRAW_UP_DOUBLE_RIGHT,            0xd3, u'\\'},
    { BOXDRAW_DOUBLE_UP_RIGHT,            0xc8, u'\\'},
    { BOXDRAW_UP_LEFT_DOUBLE,             0xbe, u'/'},
    { BOXDRAW_UP_DOUBLE_LEFT,             0xbd, u'/'},
    { BOXDRAW_DOUBLE_UP_LEFT,             0xbc, u'/'},
    { BOXDRAW_VERTICAL_RIGHT_DOUBLE,      0xc6, u'|'},
    { BOXDRAW_VERTICAL_DOUBLE_RIGHT,      0xc7, u'|'},
    { BOXDRAW_DOUBLE_VERTICAL_RIGHT,      0xcc, u'|'},
    { BOXDRAW_VERTICAL_LEFT_DOUBLE,       0xb5, u'|'},
    { BOXDRAW_VERTICAL_DOUBLE_LEFT,       0xb6, u'|'},
    { BOXDRAW_DOUBLE_VERTICAL_LEFT,       0xb9, u'|'},
    { BOXDRAW_DOWN_HORIZONTAL_DOUBLE,     0xd1, u'+'},
    { BOXDRAW_DOWN_DOUBLE_HORIZONTAL,     0xd2, u'+'},
    { BOXDRAW_DOUBLE_DOWN_HORIZONTAL,     0xcb, u'+'},
    { BOXDRAW_UP_HORIZONTAL_DOUBLE,       0xcf, u'+'},
    { BOXDRAW_UP_DOUBLE_HORIZONTAL,       0xd0, u'+'},
    { BOXDRAW_DOUBLE_UP_HORIZONTAL,       0xca, u'+'},
    { BOXDRAW_VERTICAL_HORIZONTAL_DOUBLE, 0xd8, u'+'},
    { BOXDRAW_VERTICAL_DOUBLE_HORIZONTAL, 0xd7, u'+'},
    { BOXDRAW_DOUBLE_VERTICAL_HORIZONTAL, 0xce, u'+'},

    { BLOCKELEMENT_FULL_BLOCK,            0xdb, u'*'},
    { BLOCKELEMENT_LIGHT_SHADE,           0xb0, u'+'},

    { GEOMETRICSHAPE_UP_TRIANGLE,         0x1e, u'^'},
    { GEOMETRICSHAPE_RIGHT_TRIANGLE,      0x10, u'>'},
    { GEOMETRICSHAPE_DOWN_TRIANGLE,       0x1f, u'v'},
    { GEOMETRICSHAPE_LEFT_TRIANGLE,       0x11, u'<'},

    /* BugBug: Left Arrow is an ESC. We can not make it print
                on a PCANSI terminal. If we can make left arrow 
                come out on PC ANSI we can add it back.

    { ARROW_LEFT,                         0x1b, u'<'},
    */

    { ARROW_UP,                           0x18, u'^'},
    
    /* BugBut: Took out left arrow so right has to go too.
       { ARROW_RIGHT,                        0x1a, u'>'},
    */      
    { ARROW_DOWN,                         0x19, u'v'},
    
    { 0x0000, 0x00, u'\0' }
};


BOOLEAN
LibIsValidTextGraphics (
    IN  CHAR16  Graphic,
    OUT CHAR8   *PcAnsi,    OPTIONAL
    OUT CHAR8   *Ascii      OPTIONAL
    )
/*++

Routine Description:

    Detects if a Unicode char is for Box Drawing text graphics.

Arguments:

    Grphic  - Unicode char to test.

    PcAnsi  - Optional pointer to return PCANSI equivalent of Graphic.

    Asci    - Optional pointer to return Ascii equivalent of Graphic.

Returns:

    TRUE if Gpaphic is a supported Unicode Box Drawing character.

--*/{
    UNICODE_TO_CHAR     *Table;

    if ((((Graphic & 0xff00) != 0x2500) && ((Graphic & 0xff00) != 0x2100))) {
     
        //
        // Unicode drawing code charts are all in the 0x25xx range, 
        //  arrows are 0x21xx
        //
        return FALSE;
    }

    for (Table = UnicodeToPcAnsiOrAscii; Table->Unicode != 0x0000; Table++) {
        if (Graphic == Table->Unicode) {
            if (PcAnsi) {
                *PcAnsi = Table->PcAnsi; 
            }
            if (Ascii) {
                *Ascii = Table->Ascii;
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOLEAN
IsValidAscii (
    IN  CHAR16  Ascii
    )
{
    if ((Ascii >= 0x20) && (Ascii <= 0x7f)) {
        return TRUE;
    }              
    return FALSE;
}

BOOLEAN
IsValidEfiCntlChar (
    IN  CHAR16  c
    )
{
    if (c == CHAR_NULL || c == CHAR_BACKSPACE || c == CHAR_LINEFEED || c == CHAR_CARRIAGE_RETURN) {
        return TRUE;
    }              
    return FALSE;
}

