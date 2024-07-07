#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main (EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	EFI_INPUT_KEY efi_input_key;
	EFI_STATUS efi_status;

	InitializeLib(image, systab);

	Print(u"HelloLib application started\n");

	Print(u"\n\n\nHit any key to exit this image\n");
	WaitForSingleEvent(ST->ConIn->WaitForKey, 0);

	uefi_call_wrapper(ST->ConOut->OutputString, 2, ST->ConOut, u"\n\n");

	efi_status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &efi_input_key);

	Print(u"ScanCode: %xh  UnicodeChar: %xh CallRtStatus: %x\n",
		efi_input_key.ScanCode, efi_input_key.UnicodeChar, efi_status);

	return EFI_SUCCESS;
}
