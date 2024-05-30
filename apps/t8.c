#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  INTN Argc, i;
  CHAR16 **Argv;

  InitializeLib(ImageHandle, SystemTable);
  Argc = GetShellArgcArgv(ImageHandle, &Argv);

  Print(u"Hello World, started with Argc=%d\n", Argc);
  for (i = 0 ; i < Argc ; ++i)
    Print(u"  Argv[%d] = '%s'\n", i, Argv[i]);

  Print(u"Bye.\n");
  return EFI_SUCCESS;
}
