#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main(
	EFI_HANDLE image_handle,
	EFI_SYSTEM_TABLE *systab
)
{
	EFI_GUID loaded_image_protocol = LOADED_IMAGE_PROTOCOL;
	EFI_STATUS efi_status;
	EFI_LOADED_IMAGE *li;
	UINTN pat = PoolAllocationType;
	VOID *void_li_p;

	InitializeLib(image_handle, systab);
	PoolAllocationType = 2; /* klooj */

	Print(u"Hello World! (0xd=0x%x, 13=%d)\n", 13, 13);

	Print(u"before InitializeLib(): PoolAllocationType=%d\n",
		pat);

	Print(u" after InitializeLib(): PoolAllocationType=%d\n",
		PoolAllocationType);

	/*
	 * Locate loaded_image_handle instance.
	 */

	Print(u"BS->HandleProtocol()  ");

	efi_status = uefi_call_wrapper(
		BS->HandleProtocol,
		3,
		image_handle,
		&loaded_image_protocol,
		&void_li_p);
	li = void_li_p;

	Print(u"%xh (%r)\n", efi_status, efi_status);

	if (efi_status != EFI_SUCCESS) {
		return efi_status;
	}

	Print(u"  li: %xh\n", li);

	if (!li) {
		return EFI_UNSUPPORTED;
	}

	Print(u"  li->Revision:        %xh\n", li->Revision);
	Print(u"  li->ParentHandle:    %xh\n", li->ParentHandle);
	Print(u"  li->SystemTable:     %xh\n", li->SystemTable);
	Print(u"  li->DeviceHandle:    %xh\n", li->DeviceHandle);
	Print(u"  li->FilePath:        %xh\n", li->FilePath);
	Print(u"  li->Reserved:        %xh\n", li->Reserved);
	Print(u"  li->LoadOptionsSize: %xh\n", li->LoadOptionsSize);
	Print(u"  li->LoadOptions:     %xh\n", li->LoadOptions);
	Print(u"  li->ImageBase:       %xh\n", li->ImageBase);
	Print(u"  li->ImageSize:       %xh\n", li->ImageSize);
	Print(u"  li->ImageCodeType:   %xh\n", li->ImageCodeType);
	Print(u"  li->ImageDataType:   %xh\n", li->ImageDataType);
	Print(u"  li->Unload:          %xh\n", li->Unload);

#if 0
typedef struct {
    UINT32                          Revision;
    EFI_HANDLE                      ParentHandle;
    struct _EFI_SYSTEM_TABLE        *SystemTable;

    // Source location of image
    EFI_HANDLE                      DeviceHandle;
    EFI_DEVICE_PATH                 *FilePath;
    VOID                            *Reserved;

    // Images load options
    UINT32                          LoadOptionsSize;
    VOID                            *LoadOptions;

    // Location of where image was loaded
    VOID                            *ImageBase;
    UINT64                          ImageSize;
    EFI_MEMORY_TYPE                 ImageCodeType;
    EFI_MEMORY_TYPE                 ImageDataType;

    // If the driver image supports a dynamic unload request
    EFI_IMAGE_UNLOAD                Unload;

} EFI_LOADED_IMAGE;
#endif

	return EFI_SUCCESS;
}
