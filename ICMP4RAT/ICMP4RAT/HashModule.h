#pragma once
#include <iostream>
#include <Windows.h>

DWORD getHashFromString(char* string);
DWORD getHashFromString2(char* string);

PDWORD getFunctionAddressByHash(char* library, DWORD hash);
PDWORD getFunctionAddressByHash2(char* library, DWORD hash);

std::string StringXOR(std::string input, int num);
