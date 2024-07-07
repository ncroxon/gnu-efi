#include <efi.h>
#include <efilib.h>

extern EFI_GUID GraphicsOutputProtocol;

#ifdef _MSC_VER
#if defined(_M_X64) || defined (_M_ARM64)
#define __SIZEOF_POINTER__ 8
#elif defined(_M_IX86) || defined(_M_ARM)
#define __SIZEOF_POINTER__ 4
#else
#error "Unknown arch"
#endif
#endif

#ifdef __GNUC__
#define be32_to_cpu(x) __builtin_bswap32(x)
#else
static inline UINT32 be32_to_cpu(const UINT32 x) {
	return ((x >> 24) & 0x000000FFUL) | ((x << 24) & 0xFF000000UL) |
			((x >> 8) & 0x0000FF00UL) | ((x << 8) & 0x00FF0000UL);
}
#endif

static void
fill_boxes(UINT32 *PixelBuffer, UINT32 Width, UINT32 Height, UINT32 Pitch,
	   EFI_GRAPHICS_PIXEL_FORMAT Format, EFI_PIXEL_BITMASK Info )
{
	UINT32 Red, Green;
	UINT32 y, x, color;

	switch(Format) {
	case PixelRedGreenBlueReserved8BitPerColor:
		Red = be32_to_cpu(0xff000000);
		Green = be32_to_cpu(0x00ff0000);
		break;
	case PixelBlueGreenRedReserved8BitPerColor:
		Red = be32_to_cpu(0x0000ff00);
		Green = be32_to_cpu(0x00ff0000);
		break;
	case PixelBitMask:
		Red = Info.RedMask;
		Green = Info.GreenMask;
		break;
	case PixelBltOnly:
		return;
	default:
		Print(u"Invalid pixel format\n");
		return;
	}

	for (y = 0; y < Height; y++) {
		color = ((y / 32) % 2 == 0) ? Red : Green;
		for (x = 0; x < Width; x++) {
			if (x % 32 == 0 && x != 0)
				color = (color == Red) ? Green : Red;
			PixelBuffer[y * Pitch + x] = color;
		}
	}
}

static void
draw_boxes(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop)
{
	int i, imax;
	EFI_STATUS rc;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
	UINTN NumPixels;
	UINT32 *PixelBuffer;
	UINT32 CopySize, BufferSize;
#if __SIZEOF_POINTER__ == 8
	UINT64 FrameBufferAddr;
#elif __SIZEOF_POINTER__ == 4
	UINT32 FrameBufferAddr;
#else
#error YOUR ARCH HERE
#endif

	if (gop->Mode) {
		imax = gop->Mode->MaxMode;
	} else {
		Print(u"gop->Mode is NULL\n");
		return;
	}

	for (i = 0; i < imax; i++) {
		UINTN SizeOfInfo;
		rc = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo,
					&info);
		if (rc == EFI_NOT_STARTED) {
			Print(u"gop->QueryMode() returned %r\n", rc);
			Print(u"Trying to start GOP with SetMode().\n");
			rc = uefi_call_wrapper(gop->SetMode, 2, gop,
				gop->Mode ? gop->Mode->Mode : 0);
			rc = uefi_call_wrapper(gop->QueryMode, 4, gop, i,
				&SizeOfInfo, &info);
		}

		if (EFI_ERROR(rc)) {
			Print(u"%d: Bad response from QueryMode: %r (%d)\n",
			      i, rc, rc);
			continue;
		}

		if (CompareMem(info, gop->Mode->Info, sizeof (*info)))
			continue;

		NumPixels = (UINTN)info->VerticalResolution
                            * (UINTN)info->PixelsPerScanLine;
		BufferSize = (UINT32)(NumPixels * sizeof(UINT32));
		if (BufferSize == gop->Mode->FrameBufferSize) {
			CopySize = BufferSize;
		} else {
			CopySize = BufferSize < gop->Mode->FrameBufferSize ?
				BufferSize : (UINT32)gop->Mode->FrameBufferSize;
			Print(u"height * pitch * pixelsize = %lu buf fb size is %lu; using %lu\n",
			      BufferSize, gop->Mode->FrameBufferSize, CopySize);
		}

		PixelBuffer = AllocatePool(BufferSize);
		if (!PixelBuffer) {
			Print(u"Allocation of 0x%08lx bytes failed.\n",
			      sizeof(UINT32) * NumPixels);
			return;
		}

		fill_boxes(PixelBuffer, info->HorizontalResolution,
			   info->VerticalResolution, info->PixelsPerScanLine,
			   info->PixelFormat, info->PixelInformation);

		if (info->PixelFormat == PixelBltOnly) {
			Print(u"No linear framebuffer on this device.\n");
			return;
		}
#if __SIZEOF_POINTER__ == 8
		FrameBufferAddr = (UINT64)gop->Mode->FrameBufferBase;
#elif __SIZEOF_POINTER__ == 4
		FrameBufferAddr = (UINT32)(UINT64)gop->Mode->FrameBufferBase;
#else
#error YOUR ARCH HERE
#endif

		CopyMem((VOID *)FrameBufferAddr, PixelBuffer, CopySize);
		return;
	}
	Print(u"Never found the active video mode?\n");
}

static EFI_STATUS
SetWatchdog(UINTN seconds)
{
	EFI_STATUS rc;
	rc = uefi_call_wrapper(BS->SetWatchdogTimer, 4, seconds, 0x1ffff,
				0, NULL);
	if (EFI_ERROR(rc)) {
		CHAR16 Buffer[64];
		StatusToString(Buffer, rc);
		Print(u"Bad response from QueryMode: %s (%d)\n", Buffer, rc);
	}
	return rc;
}

EFI_STATUS
efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab)
{
	EFI_STATUS rc;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

	InitializeLib(image_handle, systab);

	SetWatchdog(10);

	rc = LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);
	if (EFI_ERROR(rc)) {
		Print(u"Could not locate GOP: %r\n", rc);
		return rc;
	}

	if (!gop) {
		Print(u"LocateProtocol(GOP, &gop) returned %r but GOP is NULL\n", rc);
		return EFI_UNSUPPORTED;
	}

	draw_boxes(gop);

	SetWatchdog(0);
	return EFI_SUCCESS;
}
