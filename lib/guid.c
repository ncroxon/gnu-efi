/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    misc.c

Abstract:

    Misc EFI support functions



Revision History

--*/

#include "lib.h"


//
// Additional Known guids
//

#define SHELL_INTERFACE_PROTOCOL \
    { 0x47c7b223, 0xc42a, 0x11d2, {0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }

#define ENVIRONMENT_VARIABLE_ID  \
    { 0x47c7b224, 0xc42a, 0x11d2, {0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }

#define DEVICE_PATH_MAPPING_ID  \
    { 0x47c7b225, 0xc42a, 0x11d2, {0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }

#define PROTOCOL_ID_ID  \
    { 0x47c7b226, 0xc42a, 0x11d2, {0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }

#define ALIAS_ID  \
    { 0x47c7b227, 0xc42a, 0x11d2, {0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }

static EFI_GUID ShellInterfaceProtocol = SHELL_INTERFACE_PROTOCOL;
static EFI_GUID SEnvId                 = ENVIRONMENT_VARIABLE_ID;
static EFI_GUID SMapId                 = DEVICE_PATH_MAPPING_ID;
static EFI_GUID SProtId                = PROTOCOL_ID_ID;
static EFI_GUID SAliasId               = ALIAS_ID;

static struct {
    EFI_GUID        *Guid;
    WCHAR           *GuidName;
} KnownGuids[] = {
	{  &NullGuid,                                       u"G0" },
	{  &gEfiGlobalVariableGuid,                         u"EfiVar" },

	{  &VariableStoreProtocol,                          u"VarStore" },
	{  &gEfiDevicePathProtocolGuid,                     u"DevPath" },
	{  &gEfiLoadedImageProtocolGuid,                    u"LdImg" },
	{  &gEfiSimpleTextInProtocolGuid,                   u"TxtIn" },
	{  &gEfiSimpleTextOutProtocolGuid,                  u"TxtOut" },
	{  &gEfiBlockIoProtocolGuid,                        u"BlkIo" },
	{  &gEfiBlockIo2ProtocolGuid,                       u"BlkIo2" },
	{  &gEfiDiskIoProtocolGuid,                         u"DskIo" },
	{  &gEfiDiskIo2ProtocolGuid,                        u"DskIo2" },
	{  &gEfiSimpleFileSystemProtocolGuid,               u"Fs" },
	{  &gEfiLoadFileProtocolGuid,                       u"LdFile" },
	{  &gEfiDeviceIoProtocolGuid,                       u"DevIo" },
	{  &gEfiComponentNameProtocolGuid,                  u"CName" },
	{  &gEfiComponentName2ProtocolGuid,                 u"CName2" },

    {  &gEfiMpServicesProtocolGuid,                     u"Mp" },

	{  &gEfiFileInfoGuid,                               u"FileInfo" },
	{  &gEfiFileSystemInfoGuid,                         u"FsInfo" },
	{  &gEfiFileSystemVolumeLabelInfoIdGuid,            u"FsVolInfo" },

	{  &gEfiUnicodeCollationProtocolGuid,               u"Unicode" },
	{  &LegacyBootProtocol,                             u"LegacyBoot" },
	{  &gEfiSerialIoProtocolGuid,                       u"SerIo" },
	{  &VgaClassProtocol,                               u"VgaClass"},
	{  &gEfiSimpleNetworkProtocolGuid,                  u"Net" },
	{  &gEfiNetworkInterfaceIdentifierProtocolGuid,     u"Nii" },
	{  &gEfiPxeBaseCodeProtocolGuid,                    u"Pxe" },
	{  &gEfiPxeBaseCodeCallbackProtocolGuid,            u"PxeCb" },

	{  &TextOutSpliterProtocol,                         u"TxtOutSplit" },
	{  &ErrorOutSpliterProtocol,                        u"ErrOutSplit" },
	{  &TextInSpliterProtocol,                          u"TxtInSplit" },
	{  &gEfiPcAnsiGuid,                                 u"PcAnsi" },
	{  &gEfiVT100Guid,                                  u"Vt100" },
	{  &gEfiVT100PlusGuid,                              u"Vt100Plus" },
	{  &gEfiVTUTF8Guid,                                 u"VtUtf8" },
	{  &UnknownDevice,                                  u"UnknownDev" },

	{  &EfiPartTypeSystemPartitionGuid,                 u"ESP" },
	{  &EfiPartTypeLegacyMbrGuid,                       u"GPT MBR" },

	{  &ShellInterfaceProtocol,                         u"ShellInt" },
	{  &SEnvId,                                         u"SEnv" },
	{  &SProtId,                                        u"ShellProtId" },
	{  &SMapId,                                         u"ShellDevPathMap" },
	{  &SAliasId,                                       u"ShellAlias" },

	{  NULL, u"" }
};

//
//
//

LIST_ENTRY          GuidList;


VOID
InitializeGuid (
    VOID
    )
{
}

BOOLEAN
EFIAPI
CompareGuid_1 (
    IN CONST EFI_GUID     *Guid1,
    IN CONST EFI_GUID     *Guid2
    )
/*++

Routine Description:

    Compares to GUIDs

Arguments:

    Guid1       - guid to compare
    Guid2       - guid to compare

Returns:
    = 1     if Guid1 == Guid2

--*/
{
    return RtCompareGuid (Guid1, Guid2);
}


VOID
GuidToString (
    OUT CHAR16      *Buffer,
    IN EFI_GUID     *Guid
    )
{

    UINTN           Index;

    //
    // Else, (for now) use additional internal function for mapping guids
    //

    for (Index=0; KnownGuids[Index].Guid; Index++) {
        if (CompareGuid(Guid, KnownGuids[Index].Guid)) {
            UnicodeSPrint (Buffer, 0, KnownGuids[Index].GuidName);
            return ;
        }
    }

    //
    // Else dump it
    //

    UnicodeSPrint (Buffer, 0, u"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        Guid->Data1,
        Guid->Data2,
        Guid->Data3,
        Guid->Data4[0],
        Guid->Data4[1],
        Guid->Data4[2],
        Guid->Data4[3],
        Guid->Data4[4],
        Guid->Data4[5],
        Guid->Data4[6],
        Guid->Data4[7]
        );
}
