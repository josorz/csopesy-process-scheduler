#include<iostream>
#include<string>

#include "MainConsole.h"


int main() {
    MainConsole *mainConsole = new MainConsole();

    mainConsole->printHeading();
    mainConsole->drawConsole();

    delete mainConsole;
    return 0;
}
