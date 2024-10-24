#include <iostream>
#include <thread>
#include <string>

#include <vector>
#include <mutex>

#include "ProcessManager.h"
#include "FCFSScheduler.h"
#include "Scheduler.h"

std::mutex ProcessManagerMutex;

ProcessManager::~ProcessManager() {
    delete scheduler;
}

// func to create new process and display its console
void ProcessManager::createProcess(const std::string& name) {
    Process newProcess(name);
    this->processes.push_back(newProcess);
    scheduler->addProcess(newProcess);
    newProcess.drawConsole();
}

// find a process by name
Process* ProcessManager::findProcess(const std::string& name) {
    return scheduler->findProcess(name);
}

// redraw console for an existing process
void ProcessManager::redrawProcess(const std::string& name) {
    if (Process *process = findProcess(name)) {
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

void ProcessManager::listProcess() {
    scheduler->listProcess();
}

void ProcessManager::testScheduler() {
    scheduler->test();
}

void ProcessManager::stopScheduler() {
    scheduler->stop();
}