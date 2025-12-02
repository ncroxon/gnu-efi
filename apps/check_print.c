#include "lib.h"

void check_len_sprint(void) {
    CHAR8 buf1[200];
    CHAR16 buf2[200];
    UINTN len1;
    UINTN len2;
    CONST CHAR8 *str1 = (CHAR8*)"This is a test\n";
    CONST CHAR16 *str2 = u"This is a test\n";
    len1 = AsciiSPrint(buf1, sizeof(buf1), (CHAR8*)"%a\n", str1);
    len2 = UnicodeSPrint(buf2, sizeof(buf2), u"%s\n", str2);
    if (len1 != len2) { Print(u"Length Error!\r\n"); }
    if (len1 == len2) { Print(u"Length OK!\r\n"); }
    Print(u"%u\r\n", len1);
    Print(u"%u\r\n", len2);
}

EFI_STATUS
efi_main (EFI_HANDLE image EFI_UNUSED, EFI_SYSTEM_TABLE *systab EFI_UNUSED)
{
    check_len_sprint();
    return EFI_SUCCESS;
}

