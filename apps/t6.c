#include <efi.h>
#include <efilib.h>

typedef EFI_STATUS (*foo_t)(EFI_HANDLE, EFI_GUID *, VOID **);
typedef struct {
	unsigned long addr;
	unsigned long gp;
} fdesc_t;

EFI_LOADED_IMAGE my_loaded;

EFI_STATUS
efi_main (EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	EFI_LOADED_IMAGE *loaded_image = NULL;
#if 0
	EFI_DEVICE_PATH *dev_path;
#endif
	EFI_STATUS status;

	InitializeLib(image, systab);
	status = uefi_call_wrapper(systab->BootServices->HandleProtocol,
				3,
				image, 
				&LoadedImageProtocol, 
				(void **) &loaded_image);
	if (EFI_ERROR(status)) {
		Print(u"handleprotocol: %r\n", status);
	}

#if 0
	BS->HandleProtocol(loaded_image->DeviceHandle, &DevicePathProtocol, (void **) &dev_path);

	Print(u"Image device      : %s\n", DevicePathToStr(dev_path));
	Print(u"Image file        : %s\n", DevicePathToStr(loaded_image->FilePath));
#endif
	Print(u"Image base        : %lx\n", loaded_image->ImageBase);
	Print(u"Image size        : %lx\n", loaded_image->ImageSize);
	Print(u"Load options size : %lx\n", loaded_image->LoadOptionsSize);
	Print(u"Load options      : %s\n", loaded_image->LoadOptions);

	return EFI_SUCCESS;
}
