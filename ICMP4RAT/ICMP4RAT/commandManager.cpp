#include <Windows.h>
#include <iostream>
#include "commandManager.h"

// 이쪽 다시 구현해야할듯
void commandManager::fileListing() {
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFileW(L"C:\\*", &data);      

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::wcout << data.cFileName << std::endl;
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }
}