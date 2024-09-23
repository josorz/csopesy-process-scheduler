#pragma once

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <chrono>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <functional>

// Utility Functions
namespace Utils {

    // used for now
    int generateRandomNumber(int lower, int upper) {
        srand(time(0));
        return rand() % (upper - lower + 1) + lower;
    }

    // get current timestamp as a string
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm;
        localtime_s(&local_tm, &now_c);
        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%m/%d/%Y, %I:%M:%S %p");
        return oss.str();
    }

    // screen loop func
    void runScreenLoop(std::function<void()> action) {
        std::string command;
        while (command != "exit") {
            system("cls");
            action();
            std::cout << "\n  Type 'exit' to return back to main menu: ";
            std::getline(std::cin, command);
        }
        system("cls");
    }
}

// Process Class
class Process {
private:
    std::string processName;
    int currentLine;
    int totalLines;
    std::string creationTime;

public:
    // initializes process with a name and random instruction count
    Process(std::string name) {
        processName = name;
        totalLines = Utils::generateRandomNumber(100, 200);
        currentLine = totalLines - Utils::generateRandomNumber(20, 50);
        creationTime = Utils::getCurrentTimestamp();
    }

    // display process information on screen
    void drawConsole() {
        auto showDetails = [&]() {
            std::cout << "\n  --- SCREEN: " << processName << " ---\n";
            std::cout << "  Process name: " << processName << "\n";
            std::cout << "  Current Instruction: " << currentLine << '/' << totalLines << "\n";
            std::cout << "  Created At: " << creationTime << "\n";
            std::cout << "  ---------------------------\n";
            };
        Utils::runScreenLoop(showDetails);
    }

    const std::string& getName() const { return processName; }
};

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
            auto showError = []() {
                std::cout << "\n  Process not found! Cannot redraw\n";
                };
            Utils::runScreenLoop(showError);
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
