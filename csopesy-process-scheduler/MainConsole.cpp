#include <iostream>
#include <string>
#include "MainConsole.h"

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

void MainConsole::readCommand(std::string input) {
    if (input == "initialize") {
        std::cout << "\n  initialize command recognized. Doing something...\n";
    }
    else if (input.substr(0, 9) == "screen -s") {
        // create a new process screen
        processManager.createProcess(input.substr(10));
    }
    else if (input.substr(0, 9) == "screen -r") {
        // redraw existing process screen
        processManager.redrawProcess(input.substr(10));
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