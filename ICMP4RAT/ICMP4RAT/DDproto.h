#pragma once

#define DDPROTO_HEADER 0xdd
#define BEACON_DATA NULL
#define BEACON_DATA_LEN 0
#define MAX_DATA_LEN 0xffff


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
	ftpRequest,
	ftpResponse,
	none = 0x10
};