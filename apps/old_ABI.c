#define GNU_EFI_3_0_COMPAT

#include <efi.h>
#include <efilib.h>

/* EFI_TIMESTAMP_PROTOCOL */
#define TEST_GUID { 0xafbfde41, 0x2e6e, 0x4262,\
  { 0xba, 0x65, 0x62, 0xb9, 0x23, 0x6e, 0x54, 0x95 }}

EFI_STATUS
efi_main (EFI_HANDLE image EFI_UNUSED, EFI_SYSTEM_TABLE *systab EFI_UNUSED)
{

    CHAR16 *Dest = 0;
    EFI_UNUSED CHAR16 *Copy = 0;
    UINTN test_str_size = 0;
    CONST CHAR16 *test_str = u"Hello World!";

    test_str_size = StrSize(test_str);
    Dest = AllocatePool(test_str_size);
    CopyMem(Dest, test_str, test_str_size);
    Copy = ReallocatePool(Dest, test_str_size, test_str_size+10);
    Print(u"Done CopyMem and ReallocatePool!\r\n");

    EFI_GUID a = TEST_GUID;
    EFI_GUID b = TEST_GUID;
    if (CompareGuid(&a, &b) == 0) {
        Print(u"CompareGuid ok!\r\n");
    } else {
        Print(u"CompareGuid fail!\r\n");
    }
    return EFI_SUCCESS;
}
