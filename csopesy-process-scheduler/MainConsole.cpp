#include <iostream>
#include <string>

#include "MainConsole.h"
#include "FCFSScheduler.h"
#include "ProcessManager.h"

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

void MainConsole::readCommand(std::string input) {
    if (input == "initialize") {
        std::cout << "\n  initialize command recognized. Doing something...\n";
        if (!this->isInitialized()) {
            processManager.init();
            initialized = true;
        }
    }
    else if (input.substr(0, 10) == "screen -ls") {
        if (!this->isInitialized()) {
            std::cout << "Process scheduler uninitialized. Run initialize first.\n";
            return;
        }

        processManager.listProcess();
    }
    else if (input.substr(0, 9) == "screen -s") {
        if (!this->isInitialized()) {
            std::cout << "Process scheduler uninitialized. Run initialize first.\n";
            return;
        }

        // Validate input length
        if (input.length() < 11) {
            std::cout << "Error: No command provided for screen -s.\n";
            return;
        }

        std::string screenName = input.substr(10);

        // lazy whitespace check
        if (screenName.c_str()[0] == ' ') { 
            std::cout << "Error: No command provided for screen -s.\n";
            return;
        }

        processManager.createProcess(screenName);
    }
    else if (input.substr(0, 9) == "screen -r") {
        if (!this->isInitialized()) {
            std::cout << "Process scheduler uninitialized. Run initialize first.\n";
            return;
        }
        
        if (input.length() < 11) {
            std::cout << "Error: No command provided for screen -s.\n";
            return;
        }

        std::string screenName = input.substr(10);

        // lazy whitespace check
        if (screenName.c_str()[0] == ' ') {
            std::cout << "Error: No command provided for screen -s.\n";
            return;
        }

        processManager.redrawProcess(screenName);
    }
    else if (input == "scheduler-test") {
        if (!this->isInitialized()) {
            std::cout << "Process scheduler uninitialized. Run initialize first.\n";
            return;
        }
        processManager.testScheduler();
        std::cout << "Scheduler test ongoing. Continuously generating dummy processes.\n";
    }
    else if (input == "scheduler-stop") {
        if (!this->isInitialized()) {
            std::cout << "Process scheduler uninitialized. Run initialize first.\n";
            return;
        }
        processManager.stopScheduler();
        std::cout << "Stopped generating dummy processes.\n";
    }
    else if (input == "report-util") {
        if (!this->isInitialized()) {
            std::cout << "Process scheduler uninitialized. Run initialize first.\n";
            return;
        }
        processManager.listProcessToFile();
    }
    else if (input == "clear") {
        system("cls");
        this->printHeading();
    }
    else {
        std::cout << "\n  '" << input << "' is not recognized as an internal or external command.\n";
    }
}