#include <iostream>
#include <thread>
#include <string>

#include <vector>

#include "ProcessManager.h"
#include "FCFSScheduler.h"
#include "Scheduler.h"

std::mutex ProcessManagerMutex;

ProcessManager::~ProcessManager() {
    delete scheduler;
}

// find a process by name
Process* ProcessManager::findProcess(const std::string& name) {
    return scheduler->findProcess(name);
}

// func to create new process and display its console
void ProcessManager::createProcess(const std::string& name) {
    Process newProcess(name, min_ins, max_ins);
    this->processes.push_back(newProcess);
    scheduler->addProcess(newProcess);

    Process* process = &this->processes.back();

    std::string command;
    std::cout << "\n  Process '" << name << "' successfully created!\n";
    std::cout << "\n  Type 'process-smi' to see more information about this process: ";
    std::cout << "\n  Type 'exit' to return back to main menu: ";
    std::getline(std::cin, command);

    if (command != "exit") {
        redrawProcess(name);
    }
}

// redraw console for an existing process
void ProcessManager::redrawProcess(const std::string& name) {
    if (Process *process = findProcess(name)) {
        process->drawConsole();
    }
    else {
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

void ProcessManager::listProcessToFile() {
    scheduler->listProcessToFile();
}


void ProcessManager::testScheduler() {
    scheduler->test();
}

void ProcessManager::stopScheduler() {
    scheduler->stop();
}