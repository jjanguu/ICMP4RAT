#pragma once
#include "HashModule.h"

using customWinHttpOpen = HINTERNET(NTAPI*)(
    LPCWSTR pszAgentW,
    DWORD dwAccessType,
    LPCWSTR pszProxyW,
    LPCWSTR pszProxyBypassW,
    DWORD dwFlags
    );

using customWinHttpConnect = HINTERNET(NTAPI*)(
    HINTERNET hSession,
    LPCWSTR pswzServerName,
    INTERNET_PORT nServerPort,
    DWORD dwReserved
    );

using customWinHttpOpenRequest = HINTERNET(NTAPI*)(
    HINTERNET hConnect,
    LPCWSTR pwszVerb,
    LPCWSTR pwszObjectName,
    LPCWSTR pwszVersion,
    LPCWSTR pwszReferrer OPTIONAL,
    LPCWSTR FAR* ppwszAcceptTypes OPTIONAL,
    DWORD dwFlags
    );

using customWinHttpSendRequest = BOOL(NTAPI*)(
    HINTERNET hRequest,
    LPCWSTR lpszHeaders,
    DWORD dwHeadersLength,
    LPVOID lpOptional,
    DWORD dwOptionalLength,
    DWORD dwTotalLength,
    DWORD_PTR dwContext
    );

using customWinHttpReadData = BOOL(NTAPI*)(
    HINTERNET hRequest,
    LPVOID lpBuffer,
    DWORD dwNumberOfBytesToRead,
    LPDWORD lpdwNumberOfBytesRead
    );

using customWinHttpReceiveResponse = BOOL(NTAPI*)(
    HINTERNET hRequest,
    LPVOID lpReserved
    );

using customWinHttpQueryDataAvailable = BOOL(NTAPI*)(
    HINTERNET hRequest,
   LPDWORD lpdwNumberOfBytesAvailable
    );

using customWinHttpCloseHandle = BOOL(NTAPI*)(
    HINTERNET hInternet
    );

using customGetLastError = DWORD(NTAPI*)(
    VOID
    );

customWinHttpOpen WinHttpOpen_c = (customWinHttpOpen)getFunctionAddressByHash((char*)"Winhttp", 0x89ad2462);
customWinHttpConnect WinHttpConnect_c = (customWinHttpConnect)getFunctionAddressByHash((char*)"Winhttp", 0x4157d630);
customWinHttpOpenRequest WinHttpOpenRequest_c = (customWinHttpOpenRequest)getFunctionAddressByHash((char*)"Winhttp", 0xe0bf9f30);
customWinHttpSendRequest WinHttpSendRequest_c = (customWinHttpSendRequest)getFunctionAddressByHash((char*)"Winhttp", 0x572e1f30);
customWinHttpReadData WinHttpReadData_c = (customWinHttpReadData)getFunctionAddressByHash((char*)"Winhttp", 0x46dcc8c1);
customWinHttpReceiveResponse WinHttpReceiveResponse_c = (customWinHttpReceiveResponse)getFunctionAddressByHash((char*)"Winhttp", 0x27dfd631);
customWinHttpQueryDataAvailable WinHttpQueryDataAvailable_c = (customWinHttpQueryDataAvailable)getFunctionAddressByHash((char*)"Winhttp", 0x486c49f5);
customWinHttpCloseHandle WinHttpCloseHandle_c = (customWinHttpCloseHandle)getFunctionAddressByHash((char*)"Winhttp", 0x16cc0655);
customGetLastError GetLastError_c = (customGetLastError)getFunctionAddressByHash((char*)"Kernel32", 0x522541ae);