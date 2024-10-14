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

#include "Process.h"

// Process Management
namespace ProcessManager {
    std::vector<Process> processes;

    // func to create new process and display its console
    void createProcess(const std::string& name) {
        Process newProcess(name);
        processes.push_back(newProcess);
        newProcess.drawConsole();
    }

    // find a process by name
    Process* findProcess(const std::string& name) {
        for (auto& process : processes) {
            if (process.getName() == name) {
                return &process;
            }
        }
        return nullptr;
    }

    // redraw console for an existing process
    void redrawProcess(const std::string& name) {
        if (Process* process = findProcess(name)) {
            process->drawConsole();
        }
        else {
            // TODO: replace with proper getline
            std::string command;
            while (command != "exit") {
                system("cls");
                
                std::cout << "\n  Process not found! Cannot redraw\n";

                std::cout << "\n  Type 'exit' to return back to main menu: ";
                std::getline(std::cin, command);
            }
            system("cls");
        }
    }
}

// Command Handlers
void read_command(std::string input) {
    if (input == "initialize") {
        std::cout << "\n  initialize command recognized. Doing something...\n";
        system("cls");
    }
    else if (input.substr(0, 9) == "screen -s") {
        // create a new process screen
        ProcessManager::createProcess(input.substr(10));
    }
    else if (input.substr(0, 9) == "screen -r") {
        // redraw existing process screen
        ProcessManager::redrawProcess(input.substr(10));
    }
    else if (input == "scheduler-test") {
        std::cout << "\n  scheduler-test command recognized. Doing something...\n";
        Sleep(2000);
        system("cls");
    }
    else if (input == "scheduler-stop") {
        std::cout << "\n  scheduler-stop command recognized. Doing something...\n";
        Sleep(2000);
        system("cls");
    }
    else if (input == "report-util") {
        std::cout << "\n  report-util command recognized. Doing something...\n";
        Sleep(2000);
        system("cls");
    }
    else if (input == "clear") {
        system("cls");
    }
    else {
        std::cout << "\n  '" << input << "' is not recognized as an internal or external command. Clearing menu...\n";
        Sleep(2000);
        system("cls");
    }
}
