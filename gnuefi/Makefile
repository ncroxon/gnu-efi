#
#  Copyright (C) 1999-2001 Hewlett-Packard Co.
#	Contributed by David Mosberger <davidm@hpl.hp.com>
#	Contributed by Stephane Eranian <eranian@hpl.hp.com>
#
#    All rights reserved.
#
#    Redistribution and use in source and binary forms, with or without
#    modification, are permitted provided that the following conditions
#    are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials
#      provided with the distribution.
#    * Neither the name of Hewlett-Packard Co. nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#
#    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
#    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
#    BE LIABLE FOR ANYDIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
#    OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
#    TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
#    THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#    SUCH DAMAGE.
#

SRCDIR = .

VPATH = $(SRCDIR)
TOPDIR = $(SRCDIR)/..

include $(SRCDIR)/../Make.defaults

PKGCONFIGDIR ?= $(LIBDIR)/pkgconfig
FILES	= reloc_$(ARCH)  

OBJS	= $(FILES:%=%.o)

ifneq ($(HAVE_EFI_OBJCOPY),)
  gnu_efi_default_is_objcopy=true
else
  gnu_efi_default_is_objcopy=false
endif
# https://uefi.org/specs/UEFI/2.10/03_Boot_Manager.html#uefi-image-types
ifeq ($(ARCH),x86_64)
  efi_machine_type_name=x64
endif
ifeq ($(ARCH),ia32)
  efi_machine_type_name=ia32
endif
ifeq ($(ARCH),arm)
  efi_machine_type_name=arm
endif
ifeq ($(ARCH),aarch64)
  efi_machine_type_name=aa64
endif
ifeq ($(ARCH),ia64)
  efi_machine_type_name=ia64
endif
ifeq ($(ARCH),riscv64)
  efi_machine_type_name=riscv64
endif
ifeq ($(ARCH),loongarch64)
  efi_machine_type_name=loongarch64
endif
ifeq ($(ARCH),mips64el)
  efi_machine_type_name=mips64
endif

# on aarch64, avoid jump tables before all relocations have been processed
reloc_aarch64.o: CFLAGS += -fno-jump-tables

ifneq (,$(filter $(ARCH),aarch64 riscv64 ia32))
CRT0_LOCAL_TARGET = crt0-efi-$(ARCH)-local.o
endif

TARGETS	= crt0-efi-$(ARCH).o libgnuefi.a $(CRT0_LOCAL_TARGET)

all:	$(TARGETS) gnu-efi.pc

libgnuefi.a: $(OBJS)
	@$(ECHO) "  AR       $(notdir $@)"
	$(HIDE)$(AR) $(ARFLAGS) $@ $^

gnu-efi.pc:
	@$(ECHO) "  GEN      $(notdir $@)"
	$(HIDE)sed \
		-e 's:@PREFIX@:$(PREFIX):g' \
		-e 's:@EXEC_PREFIX@:$(EXEC_PREFIX):g' \
		-e 's:@INCLUDEDIR@:$(INCLUDEDIR):g' \
		-e 's:@LIBDIR@:$(LIBDIR):g' \
		-e 's:@VERSION@:$(VERSION):g' \
		-e 's:@efi_machine_type_name@:$(efi_machine_type_name):g' \
		-e 's:@gnu_efi_arch@:$(ARCH):g' \
		-e 's:@gnu_efi_default_is_objcopy@:$(gnu_efi_default_is_objcopy):g' \
	$(SRCDIR)/gnu-efi.pc.in > gnu-efi.pc


clean:
	@rm -vf $(TARGETS) *~ *.o $(OBJS) gnu-efi.pc

install:
	mkdir -p $(INSTALLROOT)$(LIBDIR)
	$(INSTALL) -m 644 $(TARGETS) $(INSTALLROOT)$(LIBDIR)
ifneq (,$(filter $(ARCH),x86_64 ia32))
	$(INSTALL) -m 644 $(SRCDIR)/elf_$(ARCH)_fbsd_efi.lds $(INSTALLROOT)$(LIBDIR)
endif
ifneq (,$(filter $(ARCH),aarch64 riscv64 ia32))
	$(INSTALL) -m 644 $(SRCDIR)/elf_$(ARCH)_efi_local.lds $(INSTALLROOT)$(LIBDIR)
endif
	$(INSTALL) -m 644 $(SRCDIR)/elf_$(ARCH)_efi.lds $(INSTALLROOT)$(LIBDIR)
	$(INSTALL) -d $(INSTALLROOT)$(PKGCONFIGDIR)
	$(INSTALL) -m 644 gnu-efi.pc $(INSTALLROOT)$(PKGCONFIGDIR)

include $(SRCDIR)/../Make.rules
