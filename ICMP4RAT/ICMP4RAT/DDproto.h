#pragma once
#pragma pack(1)

#define DDPROTO_HEADER 0xdd
#define BEACON_DATA NULL
#define BEACON_DATA_LEN 0
#define DATA_BUF_SIZE 0x1C200000 //450MB
#define LAST_SEQUENCE 0xFFFFFFFF


struct DDprotocol
{
	UCHAR header = DDPROTO_HEADER;
	UCHAR type;
	DWORD len;
	DWORD seq;
};

enum DDtype {
	ACK = 0x0,
	error,
	beaconRequest,
	shellRequest,
	shellResponse,
	ftpRequest,
	ftpResponse,
	none = 0x10
};

enum DDErrorCode {
	file_error = 0x01,
};