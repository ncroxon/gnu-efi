/* SPDX-License-Identifier: GPL-2.0+ OR BSD-2-Clause */

OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)
SECTIONS {
.text 0x0 :
	{
		_text = .;
		*(.text.head)
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t.*)
                *(.plt)
		. = ALIGN(16);
	}
	_etext = .;
	_text_size = _etext - _text;
. = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));
.dynamic  :
	{ *(.dynamic) }
.data :
	ALIGN(4096)
	{
		_data = .;
		*(.sdata)
		*(.data)
		*(.data1)
		*(.data.*)
		*(.got.plt)
		*(.got)

		/*
		* Note that these aren't the using the GNU "CONSTRUCTOR" output section
		* command, so they don't start with a size.  Because of p2align and the
		* end/END definitions, and the fact that they're mergeable, they can also
		* have NULLs which aren't guaranteed to be at the end.
		*/
		. = ALIGN(16);
		__init_array_start = .;
		*(SORT(.init_array.*))
		*(.init_array)
		__init_array_end = .;
		. = ALIGN(16);
		__CTOR_LIST__ = .;
		*(SORT(.ctors.*))
		*(.ctors)
		__CTOR_END__ = .;
		. = ALIGN(16);
		__DTOR_LIST__ = .;
		*(SORT(.dtors.*))
		*(.dtors)
		__DTOR_END__ = .;
		. = ALIGN(16);
		__fini_array_start = .;
		*(SORT(.fini_array.*))
		*(.fini_array)
		__fini_array_end = .;


		/* the EFI loader doesn't seem to like a .bss section, so we stick
		   it all into .data: */
		. = ALIGN(16);
		_bss = .;
		*(.sbss)
		*(.scommon)
		*(.dynbss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(16);
		_bss_end = .;
	}

. = ALIGN(4096);
.rela :
  {
    *(.rela.text*)
    *(.rela.data*)
    *(.rela.got)
    *(.rela.dyn)
    *(.rela.stab)
    *(.rela.init_array*)
    *(.rela.fini_array*)
    *(.rela.ctors*)
    *(.rela.dtors*)
  }
. = ALIGN(4096);
.rela.plt : { *(.rela.plt) }
. = ALIGN(4096);
.rodata : { *(.rodata*) }
	_edata = .;
	_data_size = _edata - _data;

	. = ALIGN(4096);
.dynsym   :
	{ *(.dynsym) }
	. = ALIGN(4096);
.dynstr   :
	{ *(.dynstr) }
	. = ALIGN(4096);
.note.gnu.build-id :
	{ *(.note.gnu.build-id) }
. = DATA_SEGMENT_END (.);
/DISCARD/ :
	{
		*(.rel.reloc)
		*(.eh_frame)
		*(.note.GNU-stack)
	}
.comment 0 :
	{ *(.comment) }
}
