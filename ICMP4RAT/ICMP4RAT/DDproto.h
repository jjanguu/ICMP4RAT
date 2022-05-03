#pragma once
struct DDprotocol
{
	unsigned char header = 0xDD;
	unsigned char type;
	unsigned short len;
	unsigned int seq;

};

enum DDtype {
	error = 0,
	beaconRequest,
	beaconResponse,
	shellRequest,
	shellResponse,
	ftpReqeust,
	ftpResponse
};