#pragma once

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <functional>

#include "ProcessManager.h"
#include "Process.h"

// Command Handlers
void read_command(std::string input, ProcessManager *processManager) {
    if (input == "initialize") {
        std::cout << "\n  initialize command recognized. Doing something...\n";
    }
    else if (input.substr(0, 9) == "screen -s") {
        // create a new process screen
        processManager->createProcess(input.substr(10));
    }
    else if (input.substr(0, 9) == "screen -r") {
        // redraw existing process screen
        processManager->redrawProcess(input.substr(10));
    }
    else if (input == "scheduler-test") {
        std::cout << "\n  scheduler-test command recognized. Doing something...\n";
    }
    else if (input == "scheduler-stop") {
        std::cout << "\n  scheduler-stop command recognized. Doing something...\n";
    }
    else if (input == "report-util") {
        std::cout << "\n  report-util command recognized. Doing something...\n";
    }
    else if (input == "clear") {
        system("cls");
    }
    else {
        std::cout << "\n  '" << input << "' is not recognized as an internal or external command.\n";
    }
}
