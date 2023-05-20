/* SPDX-License-Identifier: GPL-2.0+ OR BSD-2-Clause */
/*
 * Copright (C) 2014 Linaro Ltd. <ard.biesheuvel@linaro.org>
 * Copright (C) 2018 Alexander Graf <agraf@suse.de>
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

#ifndef EFI_SUBSYSTEM
#define EFI_SUBSYSTEM 10
#endif

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
	.2byte	0x5064				// riscv64
	.2byte	2				// nr_sections
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
	.4byte	_data - _start			// SizeOfCode
	.4byte	_data_size			// SizeOfInitializedData
	.4byte	0				// SizeOfUninitializedData
	.4byte	_start - ImageBase		// AddressOfEntryPoint
	.4byte	_start - ImageBase		// BaseOfCode

extra_header_fields:
	.8byte	0				// ImageBase
	.4byte	0x1000				// SectionAlignment
	.4byte	0x200				// FileAlignment
	.2byte	0				// MajorOperatingSystemVersion
	.2byte	0				// MinorOperatingSystemVersion
	.2byte	0				// MajorImageVersion
	.2byte	0				// MinorImageVersion
	.2byte	0				// MajorSubsystemVersion
	.2byte	0				// MinorSubsystemVersion
	.4byte	0				// Win32VersionValue

	.4byte	_edata - ImageBase		// SizeOfImage

	// Everything before the kernel image is considered part of the header
	.4byte	_start - ImageBase		// SizeOfHeaders
	.4byte	0				// CheckSum
	.2byte	EFI_SUBSYSTEM			// Subsystem
	.2byte	0				// DllCharacteristics
	.8byte	0				// SizeOfStackReserve
	.8byte	0				// SizeOfStackCommit
	.8byte	0				// SizeOfHeapReserve
	.8byte	0				// SizeOfHeapCommit
	.4byte	0				// LoaderFlags
	.4byte	0x6				// NumberOfRvaAndSizes

	.8byte	0				// ExportTable
	.8byte	0				// ImportTable
	.8byte	0				// ResourceTable
	.8byte	0				// ExceptionTable
	.8byte	0				// CertificationTable
	.8byte	0				// BaseRelocationTable

	// Section table
section_table:
	/*
	 * The EFI application loader requires a relocation section
	 * because EFI applications must be relocatable.  This is a
	 * dummy section as far as we are concerned.
	 */
	.ascii	".reloc\0\0"
	.4byte	0
	.4byte	0
	.4byte	0				// SizeOfRawData
	.4byte	0				// PointerToRawData
	.4byte	0				// PointerToRelocations
	.4byte	0				// PointerToLineNumbers
	.2byte	0				// NumberOfRelocations
	.2byte	0				// NumberOfLineNumbers
	.4byte	0x42100040			// Characteristics (section flags)

	.ascii	".text\0\0\0"
	.4byte	_edata - _start			// VirtualSize
	.4byte	_start - ImageBase		// VirtualAddress
	.4byte	_edata - _start			// SizeOfRawData
	.4byte	_start - ImageBase		// PointerToRawData

	.4byte	0				// PointerToRelocations (0 for executables)
	.4byte	0				// PointerToLineNumbers (0 for executables)
	.2byte	0				// NumberOfRelocations  (0 for executables)
	.2byte	0				// NumberOfLineNumbers  (0 for executables)
	.4byte	0xe0500020			// Characteristics (section flags)

	.align	12
	.globl _start
_start:
	addi		sp, sp, -24
	sd		a0, 0(sp)
	sd		a1, 8(sp)
	sd		ra, 16(sp)
	lla		a0, ImageBase
	lla		a1, _DYNAMIC
	call		_relocate
	bne		a0, zero, 0f
	ld		a1, 8(sp)
	ld		a0, 0(sp)
	call		efi_main
	ld		ra, 16(sp)
0:	addi		sp, sp, 24
	ret

#if defined(__ELF__) && defined(__linux__)
	.section .note.GNU-stack,"",%progbits
#endif
