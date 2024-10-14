#include <iostream>
#include <string>

#include "ProcessManager.h"

// func to create new process and display its console
void ProcessManager::createProcess(const std::string& name) {
    Process newProcess(name);
    this->processes.push_back(newProcess);
    newProcess.drawConsole();
}

// find a process by name
Process* ProcessManager::findProcess(const std::string& name) {
    for (auto& process : this->processes) {
        if (process.getName() == name) {
            return &process;
        }
    }
    return nullptr;
}

// redraw console for an existing process
void ProcessManager::redrawProcess(const std::string& name) {
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