/*
 * crt0-efi-loongarch64-local.S - PE/COFF header for LoongArch64 EFI applications (without suitable objcopy)
 *
 * Copyright (C) 2014 Linaro Ltd. <ard.biesheuvel@linaro.org>
 * Copyright (C) 2021 Loongson Technology Corporation Limited. <zhoumingtao@loongson.cn>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice and this list of conditions, without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 */

	.section	.text.head

	/*
	 * Magic "MZ" signature for PE/COFF
	 */
	.globl	ImageBase
ImageBase:
	.ascii	"MZ"
	.skip	58				// 'MZ' + pad + offset == 64
	.4byte	pe_header - ImageBase		// Offset to the PE header.
pe_header:
	.ascii	"PE"
	.2byte 	0
coff_header:
	.2byte	0x6264				// LoongArch64
	.2byte	4				// nr_sections
	.4byte	0 				// TimeDateStamp
	.4byte	0				// PointerToSymbolTable
	.4byte	0				// NumberOfSymbols
	.2byte	section_table - optional_header	// SizeOfOptionalHeader
	.2byte	0x206				// Characteristics.
						// IMAGE_FILE_DEBUG_STRIPPED |
						// IMAGE_FILE_EXECUTABLE_IMAGE |
						// IMAGE_FILE_LINE_NUMS_STRIPPED
optional_header:
	.2byte	0x20b				// PE32+ format
	.byte	0x02				// MajorLinkerVersion
	.byte	0x14				// MinorLinkerVersion
	.4byte	_text_size - ImageBase		// SizeOfCode
	.4byte	_alldata_size - ImageBase		// SizeOfInitializedData
	.4byte	0				// SizeOfUninitializedData
	.4byte	_start - ImageBase		// AddressOfEntryPoint
	.4byte	_text - ImageBase		// BaseOfCode

extra_header_fields:
	.8byte	0				// ImageBase
	.4byte	0x1000				// SectionAlignment
	.4byte	0x1000				// FileAlignment
	.2byte	0				// MajorOperatingSystemVersion
	.2byte	0				// MinorOperatingSystemVersion
	.2byte	0				// MajorImageVersion
	.2byte	0				// MinorImageVersion
	.2byte	0				// MajorSubsystemVersion
	.2byte	0				// MinorSubsystemVersion
	.4byte	0				// Win32VersionValue

	.4byte	_image_end - ImageBase		// SizeOfImage

	// Everything before the kernel image is considered part of the header
	.4byte	_text - ImageBase		// SizeOfHeaders
	.4byte	0				// CheckSum
	.2byte	EFI_SUBSYSTEM			// Subsystem
	.2byte	0				// DllCharacteristics
	.8byte	0				// SizeOfStackReserve
	.8byte	0				// SizeOfStackCommit
	.8byte	0				// SizeOfHeapReserve
	.8byte	0				// SizeOfHeapCommit
	.4byte	0				// LoaderFlags
	.4byte	0x10				// NumberOfRvaAndSizes

	.8byte	0				// ExportTable
	.8byte	0				// ImportTable
	.8byte	0				// ResourceTable
	.8byte	0				// ExceptionTable
	.8byte	0				// CertificationTable
	.4byte	_reloc - ImageBase				// BaseRelocationTable (VirtualAddress)
	.4byte	_reloc_vsize - ImageBase				// BaseRelocationTable (Size)
	.8byte	0				// Debug
	.8byte	0				// Architecture
	.8byte	0				// Global Ptr
	.8byte	0				// TLS Table
	.8byte	0				// Load Config Table
	.8byte	0				// Bound Import
	.8byte	0				// IAT
	.8byte	0				// Delay Import Descriptor
	.8byte	0				// CLR Runtime Header
	.8byte	0				// Reserved, must be zero

	// Section table
section_table:

	.ascii	".text\0\0\0"
	.4byte	_text_vsize - ImageBase		// VirtualSize
	.4byte	_text - ImageBase	// VirtualAddress
	.4byte	_text_size - ImageBase		// SizeOfRawData
	.4byte	_text - ImageBase	// PointerToRawData
	.4byte	0		// PointerToRelocations (0 for executables)
	.4byte	0		// PointerToLineNumbers (0 for executables)
	.2byte	0		// NumberOfRelocations  (0 for executables)
	.2byte	0		// NumberOfLineNumbers  (0 for executables)
	.4byte	0x60000020	// Characteristics (section flags)

	.ascii	".data\0\0\0"
	.4byte	_data_vsize - ImageBase			// VirtualSize
	.4byte	_data - ImageBase			// VirtualAddress
	.4byte	_data_size - ImageBase			// SizeOfRawData
	.4byte	_data - ImageBase			// PointerToRawData
	.4byte	0			// PointerToRelocations
	.4byte	0			// PointerToLineNumbers
	.2byte	0			// NumberOfRelocations
	.2byte	0			// NumberOfLineNumbers
	.4byte	0xC0000040		// Characteristics (section flags)

	/*
	 * The EFI application loader requires a relocation section
	 * because EFI applications must be relocatable.  This is a
	 * dummy section as far as we are concerned.
	 */
	.ascii	".reloc\0\0"
	.4byte	_reloc_vsize - ImageBase			// VirtualSize
	.4byte	_reloc - ImageBase			// VirtualAddress
	.4byte	_reloc_size - ImageBase			// SizeOfRawData
	.4byte	_reloc - ImageBase			// PointerToRawData
	.4byte	0			// PointerToRelocations
	.4byte	0			// PointerToLineNumbers
	.2byte	0			// NumberOfRelocations
	.2byte	0			// NumberOfLineNumbers
	.4byte	0x42000040		// Characteristics (section flags)

	.ascii	".rodata\0"
	.4byte	_rodata_vsize - ImageBase			// VirtualSize
	.4byte	_rodata - ImageBase			// VirtualAddress
	.4byte	_rodata_size - ImageBase			// SizeOfRawData
	.4byte	_rodata - ImageBase			// PointerToRawData
	.4byte	0			// PointerToRelocations
	.4byte	0			// PointerToLineNumbers
	.2byte	0			// NumberOfRelocations
	.2byte	0			// NumberOfLineNumbers
	.4byte	0x40000040		// Characteristics (section flags)

	.text
	.align		12
	.globl	_start
	.type	_start, @function
_start:
	addi.d	  $sp, $sp, -24
	st.d	  $ra, $sp, 0
	st.d	  $a0, $sp, 8
	st.d 	  $a1, $sp, 16

	move	  $a2, $a0		// a2: ImageHandle
	move	  $a3, $a1 		// a3: SystemTable
	la.local  $a0, ImageBase	// a0: ImageBase
	la.local  $a1, _DYNAMIC		// a1: DynamicSection
	bl        _relocate
	bnez	  $a0, .L_exit

	ld.d	  $a0, $sp, 8
	ld.d	  $a1, $sp, 16
	bl        _entry

.L_exit:
	ld.d	  $ra, $sp, 0
	addi.d	  $sp, $sp, 24
	jr	      $ra

// hand-craft a dummy .reloc section so EFI knows it's a relocatable executable:

	.data
dummy0:	.4byte	0
dummy1:	.4byte	0

#define IMAGE_REL_ABSOLUTE	0
	.section .reloc, "a", %progbits
	.4byte	dummy1 - dummy0				// Page RVA
	.4byte	12					// Block Size (2*4+2*2), must be aligned by 32 Bits
	.2byte	(IMAGE_REL_ABSOLUTE<<12) +  0		// reloc for dummy
	.2byte	(IMAGE_REL_ABSOLUTE<<12) +  0		// reloc for dummy

#if defined(__ELF__) && defined(__linux__)
	.section .note.GNU-stack,"",%progbits
#endif
