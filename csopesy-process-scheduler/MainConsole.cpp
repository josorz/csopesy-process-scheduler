#include <iostream>
#include <string>

#include "MainConsole.h"
#include "FCFSScheduler.h"
#include "ProcessManager.h"

unsigned int cpuCycles = 0;

MainConsole::MainConsole() {
}

void MainConsole::printHeading() {
    std::cout << "  _____   _____   ____   _____   ______   _____ __     __\n";
    std::cout << " / ____| / ____| / __ \\ |  __ \\ |  ____| / ____|\\ \\   / /\n";
    std::cout << "| |     | (___  | |  | || |__) || |__   | (___   \\ \\_/ / \n";
    std::cout << "| |      \\___ \\ | |  | ||  ___/ |  __|   \\___ \\   \\   /  \n";
    std::cout << "| |____  ____) || |__| || |     | |____  ____) |   | |   \n";
    std::cout << " \\_____||_____/  \\____/ |_|     |______||_____/    |_|   \n";
    std::cout << "\033[32mHello, Welcome to CSOPESY commandline!\n"; // \033[32m = green color
    std::cout << "\033[93mType \'exit\' to quit, \'clear\' to clear the screen\n";
    std::cout << "\033[0m";
}

void MainConsole::drawConsole() {
    std::string input;
    while (input != "exit") {
        // Get user input
        std::cout << "Enter a command: ";
        std::getline(std::cin, input);

        // Handle exit explicitly
        if (input == "exit") {
            std::cout << "Exiting the program...\n";
            break;
        }

        readCommand(input);
    }
}

bool MainConsole::isInitialized() {
    return this->initialized;
}

std::string MainConsole::getScreenName(std::string input, std::string command) {
    if (input.length() <= command.length()) {
        std::cout << "Error: No process specified for " << command << ".\n";
        return "";
    }

    std::string screenName = input.substr(command.length());
    screenName.erase(0, screenName.find_first_not_of(' '));

    if (screenName.empty()) {
        std::cout << "Error: No process specified for " << command << ".\n";
    }
    return screenName;
}

void MainConsole::readCommand(std::string input) {
    if (input != "initialize" && !isInitialized()) {
        std::cout << "Process scheduler uninitialized. Run initialize first.\n";
        return;
    }
    else if (input == "initialize") {
        std::cout << "\n  initialize command recognized. Doing something...\n";
        if (!isInitialized()) {
            processManager.init();
            initialized = true;
        }
    }
    else if (input.substr(0, 10) == "screen -ls") {
        processManager.listProcess();
    }
    else if (input.substr(0, 9) == "screen -s") {
        std::string screenName = getScreenName(input, "screen -s");
        if (!screenName.empty()) {
            processManager.createProcess(screenName);
        }
    }
    else if (input.substr(0, 9) == "screen -r") {
        std::string screenName = getScreenName(input, "screen -r");
        if (!screenName.empty()) {
            processManager.redrawProcess(screenName);
        }
    }
    else if (input == "scheduler-test") {
        processManager.testScheduler();
        std::cout << "Scheduler test ongoing. Continuously generating dummy processes.\n";
    }
    else if (input == "scheduler-stop") {
        processManager.stopScheduler();
        std::cout << "Stopped generating dummy processes.\n";
    }
    else if (input == "report-util") {
        processManager.listProcessToFile();
    }
    else if (input == "clear") {
        system("cls");
        printHeading();
    }
    else {
        std::cout << "\n  '" << input << "' is not recognized as an internal or external command.\n";
    }
}