#pragma once

#define DDPROTO_HEADER 0xdd

struct DDprotocol
{
	UCHAR header = DDPROTO_HEADER;
	UCHAR type;
	USHORT len;
	DWORD seq;
};

enum DDtype {
	error = 0x0,
	beaconRequest,
	beaconResponse,
	shellRequest,
	shellResponse,
	ftpReqeust,
	ftpResponse,
	none = 0x10
};