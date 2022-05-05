#include "cncManager.h"
#include "commandManager.h"
#include <stdlib.h>
#include <iostream>
#include <string>

/* For DEBUG */
int main() {
    commandManager cmd;
    cncManager client;
    client.handleShellRequest(cmd);
}