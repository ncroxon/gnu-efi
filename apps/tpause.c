#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main (EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	Print(u"Press `q' to quit, any other key to continue:\n");
	return EFI_SUCCESS;
}
