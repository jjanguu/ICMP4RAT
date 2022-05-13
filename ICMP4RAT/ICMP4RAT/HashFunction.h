#pragma once
#include "HashModule.h"

using customGetEnvironmentVariableA = LSTATUS(NTAPI*)(
    LPCSTR lpName,
    LPSTR lpBuffer,
    DWORD nSize
    ); customGetEnvironmentVariableA GetEnvironmentVariableA_c = (customGetEnvironmentVariableA)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"GetEnvironmentVariableA"));

using customCreateDirectoryA = LSTATUS(NTAPI*)(
    LPCSTR lpPathName,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes
    ); customRegOpenKeyExA RegOpenKeyExA_c = (customRegOpenKeyExA)getFunctionAddressByHash((char*)"Advapi32", getHashFromString((char*)"RegOpenKeyExA"));

using customGetModuleFileNameA = LSTATUS(NTAPI*)(
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize
    ); customRegSetValueExA RegSetValueExA_c = (customRegSetValueExA)getFunctionAddressByHash((char*)"Advapi32", getHashFromString((char*)"RegSetValueExA"));

using customRegOpenKeyExA = LSTATUS(NTAPI*)(
    HKEY hKey,
    LPCSTR lpSubKey,
    DWORD ulOptions,
    REGSAM samDesired,
    PHKEY phkResult
    ); customRegCloseKey RegCloseKey_c = (customRegCloseKey)getFunctionAddressByHash((char*)"Advapi32", getHashFromString((char*)"RegCloseKey"));

using customRegSetValueExA = LSTATUS(NTAPI*)(
    HKEY hKey,
    LPCSTR lpValueName,
    DWORD Reserved,
    DWORD dwType,
    const BYTE* lpData,
    DWORD      cbData
    ); customGetModuleFileNameA GetModuleFileNameA_c = (customGetModuleFileNameA)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"GetModuleFileNameA"));

using customRegCloseKey = LSTATUS(NTAPI*)(
    HKEY hKey
    ); customCreateDirectoryA CreateDirectoryA_c = (customCreateDirectoryA)getFunctionAddressByHash((char*)"Kernel32", getHashFromString((char*)"CreateDirectoryA"));










