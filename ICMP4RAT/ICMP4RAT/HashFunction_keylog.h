#pragma once
#include "HashModule.h"

using customSetWindowsHookExA = HHOOK(NTAPI*)(
    int idHook,
    HOOKPROC lpfn,
    HINSTANCE hmod,
    DWORD dwThreadId
    );

using customUnHookWindowsHookExA = BOOL(NTAPI*)(
    HHOOK hhk
    );

using customGetModuleHandleW = HMODULE(NTAPI*)(
    LPCWSTR lpModuleName
    );

using customGetMessageW = BOOL(NTAPI*)(
    LPMSG lpMsg,
    HWND hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax
    );

using customCallNextHookEx = LRESULT(NTAPI*)(
    HHOOK hhk,
    int nCode,
    WPARAM wParam,
    LPARAM lParam
    );

using customGetForegroundWindow = HWND(NTAPI*)(
    VOID
    );

using customGetWindowThreadProcessId = DWORD(NTAPI*)(
    HWND hWnd,
    LPDWORD lpdwProcessId
    );


customSetWindowsHookExA SetWindowsHookExA_c = (customSetWindowsHookExA)getFunctionAddressByHash((char*)"User32", 0xbaa5f631);
customUnHookWindowsHookExA UnHookWindowsHookExA_c = (customUnHookWindowsHookExA)getFunctionAddressByHash((char*)"User32", 0xa6a8459c);
customGetModuleHandleW GetModuleHandleW_c = (customGetModuleHandleW)getFunctionAddressByHash((char*)"Kernel32", 0xb9e0a54b);
customGetMessageW GetMessageW_c = (customGetMessageW)getFunctionAddressByHash((char*)"User32", 0xd7c9ce3b);
customCallNextHookEx CallNextHookEx_c = (customCallNextHookEx)getFunctionAddressByHash((char*)"User32", 0x9b6b959c);
customGetForegroundWindow GetForegroundWindow_c = (customGetForegroundWindow)getFunctionAddressByHash2((char*)"User32", 0xe9b9726b);
customGetWindowThreadProcessId GetWindowThreadProcessId_c = (customGetWindowThreadProcessId)getFunctionAddressByHash((char*)"User32", 0xe286bd98);