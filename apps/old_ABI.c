#define GNU_EFI_USE_REALLOCATEPOOL_ABI 0
#define GNU_EFI_USE_COPYMEM_ABI 0

#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main (EFI_HANDLE image EFI_UNUSED, EFI_SYSTEM_TABLE *systab EFI_UNUSED)
{

    CHAR16 *Dest = 0;
    EFI_UNUSED CHAR16 *Copy = 0;
    UINTN test_str_size = 0;
    CONST CHAR16 *test_str = L"Hello World!";

    test_str_size = StrSize(test_str);
    Dest = AllocatePool(test_str_size);
    CopyMem(Dest, test_str, test_str_size);
    Copy = ReallocatePool(Dest, test_str_size, test_str_size+10);
    Print(L"Done!\r\n");
    return EFI_SUCCESS;
}
