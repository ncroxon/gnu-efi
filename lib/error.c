/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    error.c

Abstract:




Revision History

--*/

#include "lib.h"


typedef struct {
    EFI_STATUS	Code;
    WCHAR		*Desc;
} ErrorCodeTable_Type;
ErrorCodeTable_Type ErrorCodeTable[] = {
	{  EFI_SUCCESS,                u"Success"},
	{  EFI_LOAD_ERROR,             u"Load Error"},
	{  EFI_INVALID_PARAMETER,      u"Invalid Parameter"},
	{  EFI_UNSUPPORTED,            u"Unsupported"},
	{  EFI_BAD_BUFFER_SIZE,        u"Bad Buffer Size"},
	{  EFI_BUFFER_TOO_SMALL,       u"Buffer Too Small"},
	{  EFI_NOT_READY,              u"Not Ready"},
	{  EFI_DEVICE_ERROR,           u"Device Error"},
	{  EFI_WRITE_PROTECTED,        u"Write Protected"},
	{  EFI_OUT_OF_RESOURCES,       u"Out of Resources"},
	{  EFI_VOLUME_CORRUPTED,       u"Volume Corrupt"},
	{  EFI_VOLUME_FULL,            u"Volume Full"},
	{  EFI_NO_MEDIA,               u"No Media"},
	{  EFI_MEDIA_CHANGED,          u"Media changed"},
	{  EFI_NOT_FOUND,              u"Not Found"},
	{  EFI_ACCESS_DENIED,          u"Access Denied"},
	{  EFI_NO_RESPONSE,            u"No Response"},
	{  EFI_NO_MAPPING,             u"No mapping"},
	{  EFI_TIMEOUT,                u"Time out"},
	{  EFI_NOT_STARTED,            u"Not started"},
	{  EFI_ALREADY_STARTED,        u"Already started"},
	{  EFI_ABORTED,                u"Aborted"},
	{  EFI_ICMP_ERROR,             u"ICMP Error"},
	{  EFI_TFTP_ERROR,             u"TFTP Error"},
	{  EFI_PROTOCOL_ERROR,         u"Protocol Error"},
	{  EFI_INCOMPATIBLE_VERSION,   u"Incompatible Version"},
	{  EFI_SECURITY_VIOLATION,     u"Security Policy Violation"},
	{  EFI_CRC_ERROR,              u"CRC Error"},
	{  EFI_END_OF_MEDIA,           u"End of Media"},
	{  EFI_END_OF_FILE,            u"End of File"},
	{  EFI_INVALID_LANGUAGE,       u"Invalid Languages"},
	{  EFI_COMPROMISED_DATA,       u"Compromised Data"},
	{  EFI_IP_ADDRESS_CONFLICT,    u"IP Address Conflict"},
	{  EFI_HTTP_ERROR,             u"HTTP Error"},

	// warnings
	{  EFI_WARN_UNKNOWN_GLYPH,     u"Warning Unknown Glyph"},
	{  EFI_WARN_DELETE_FAILURE,    u"Warning Delete Failure"},
	{  EFI_WARN_WRITE_FAILURE,     u"Warning Write Failure"},
	{  EFI_WARN_BUFFER_TOO_SMALL,  u"Warning Buffer Too Small"},
	{  EFI_WARN_STALE_DATA,        u"Warning Stale Data"},
	{  EFI_WARN_FILE_SYSTEM,       u"Warning File System"},
	{  EFI_WARN_RESET_REQUIRED,    u"Warning Reset Required"},
	{  0, NULL}
} ;


VOID
StatusToString (
    OUT CHAR16          *Buffer,
    IN EFI_STATUS       Status
    )
{
    UINTN           Index;

    for (Index = 0; ErrorCodeTable[Index].Desc; Index +=1) {
        if (ErrorCodeTable[Index].Code == Status) {
	    StrCpy (Buffer, ErrorCodeTable[Index].Desc);
            return;
        }
    }

    UnicodeSPrint (Buffer, 0, u"%X", Status);
}
