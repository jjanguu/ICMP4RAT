#include "HashModule.h"

DWORD getHashFromString(char* string)
{
    size_t stringLength = strnlen_s(string, 50);
    DWORD hash = 0x35;

    for (size_t i = 0; i < stringLength; i++)
    {
        hash += (hash * 0x132abd23 + string[i]) & 0xffffffff;
    }
    //printf("%s: 0x%x\n", string, hash);

    return hash;
}

DWORD getHashFromString2(char* string)
{
    size_t stringLength = strnlen_s(string, 50);
    DWORD hash = 0x32;

    for (size_t i = 0; i < stringLength; i++)
    {
        hash += (hash * 0xabcdef12 + string[i]) & 0xffffffff;
    }
    //printf("%s: 0x%x\n", string, hash);

    return hash;
}

PDWORD getFunctionAddressByHash(char* library, DWORD hash)
{
    PDWORD functionAddress = (PDWORD)0;

    // Get base address of the module in which our exported function of interest resides (kernel32 in the case of CreateThread)
    HMODULE libraryBase = LoadLibraryA(library);

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)libraryBase;
    PIMAGE_NT_HEADERS imageNTHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)libraryBase + dosHeader->e_lfanew);

    DWORD_PTR exportDirectoryRVA = imageNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

    PIMAGE_EXPORT_DIRECTORY imageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((DWORD_PTR)libraryBase + exportDirectoryRVA);

    // Get RVAs to exported function related information
    PDWORD addresOfFunctionsRVA = (PDWORD)((DWORD_PTR)libraryBase + imageExportDirectory->AddressOfFunctions);
    PDWORD addressOfNamesRVA = (PDWORD)((DWORD_PTR)libraryBase + imageExportDirectory->AddressOfNames);
    PWORD addressOfNameOrdinalsRVA = (PWORD)((DWORD_PTR)libraryBase + imageExportDirectory->AddressOfNameOrdinals);

    // Iterate through exported functions, calculate their hashes and check if any of them match our hash of 0x00544e304 (CreateThread)
    // If yes, get its virtual memory address (this is where CreateThread function resides in memory of our process)
    for (DWORD i = 0; i < imageExportDirectory->NumberOfFunctions; i++)
    {
        DWORD functionNameRVA = addressOfNamesRVA[i];
        DWORD_PTR functionNameVA = (DWORD_PTR)libraryBase + functionNameRVA;
        char* functionName = (char*)functionNameVA;
        DWORD_PTR functionAddressRVA = 0;

        // Calculate hash for this exported function
        DWORD functionNameHash = getHashFromString(functionName);

        // If hash for CreateThread is found, resolve the function address
        if (functionNameHash == hash)
        {
            functionAddressRVA = addresOfFunctionsRVA[addressOfNameOrdinalsRVA[i]];
            functionAddress = (PDWORD)((DWORD_PTR)libraryBase + functionAddressRVA);
            printf("%s : 0x%x : %p\n", functionName, functionNameHash, functionAddress);
            return functionAddress;
        }
    }
}

PDWORD getFunctionAddressByHash2(char* library, DWORD hash)
{
    PDWORD functionAddress = (PDWORD)0;

    // Get base address of the module in which our exported function of interest resides (kernel32 in the case of CreateThread)
    HMODULE libraryBase = LoadLibraryA(library);

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)libraryBase;
    PIMAGE_NT_HEADERS imageNTHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)libraryBase + dosHeader->e_lfanew);

    DWORD_PTR exportDirectoryRVA = imageNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

    PIMAGE_EXPORT_DIRECTORY imageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((DWORD_PTR)libraryBase + exportDirectoryRVA);

    // Get RVAs to exported function related information
    PDWORD addresOfFunctionsRVA = (PDWORD)((DWORD_PTR)libraryBase + imageExportDirectory->AddressOfFunctions);
    PDWORD addressOfNamesRVA = (PDWORD)((DWORD_PTR)libraryBase + imageExportDirectory->AddressOfNames);
    PWORD addressOfNameOrdinalsRVA = (PWORD)((DWORD_PTR)libraryBase + imageExportDirectory->AddressOfNameOrdinals);

    // Iterate through exported functions, calculate their hashes and check if any of them match our hash of 0x00544e304 (CreateThread)
    // If yes, get its virtual memory address (this is where CreateThread function resides in memory of our process)
    for (DWORD i = 0; i < imageExportDirectory->NumberOfFunctions; i++)
    {
        DWORD functionNameRVA = addressOfNamesRVA[i];
        DWORD_PTR functionNameVA = (DWORD_PTR)libraryBase + functionNameRVA;
        char* functionName = (char*)functionNameVA;
        DWORD_PTR functionAddressRVA = 0;

        // Calculate hash for this exported function
        DWORD functionNameHash = getHashFromString2(functionName);

        // If hash for CreateThread is found, resolve the function address
        if (functionNameHash == hash)
        {
            functionAddressRVA = addresOfFunctionsRVA[addressOfNameOrdinalsRVA[i]];
            functionAddress = (PDWORD)((DWORD_PTR)libraryBase + functionAddressRVA);
            printf("%s : 0x%x : %p\n", functionName, functionNameHash, functionAddress);
            return functionAddress;
        }
    }
}

std::string StringXOR(std::string input, int num) {
    char* path = new char[input.length()];
    for (int i = 0; i < input.length(); i++) {
        path[i] = input.c_str()[i] ^ num;
    }
    std::string result = path;
    delete[] path;
    return result;
}
