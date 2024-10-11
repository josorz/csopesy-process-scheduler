#pragma once
#include <iostream>
#include <thread>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <chrono>
#include <queue>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <functional>
#include <mutex>
#include <fstream>

void print_heading();

extern int used_core;
extern int num_cpu;
extern int total_process;

// Utility Functions
namespace Utils {
    // Get current timestamp as a string
    std::string getCurrentTimestamp() {
        time_t timestamp = time(NULL);
        struct tm datetime;
        localtime_s(&datetime, &timestamp);
        std::ostringstream oss;
        oss << std::put_time(&datetime, "%m/%d/%Y %I:%M:%S%p");
        return oss.str();
    }

    // Screen loop function
    void runScreenLoop(std::function<void()> action) {
        std::string command;
        action();
        while (command != "exit") {
            std::cout << "\n  Type 'exit' to return back to main menu: ";
            std::getline(std::cin, command);
            if (command != "exit") { std::cout << "Invalid input\n"; }
        }
        system("cls");
        print_heading();
    }
}

// Process Class
class Process {
private:
    std::string processName;
    int core;
    int currentLine;
    int totalLines;
    std::string creationTime;
    std::string finishTime;
    bool fin;

public:
    Process(std::string name) {
        processName = name;
        core = -1;
        currentLine = 0;
        totalLines = 100;
        creationTime = Utils::getCurrentTimestamp();
        fin = false;
    }

    // Display process information on screen
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
    int getCore() const { return core; }
    void setCore(int newCore) { core = newCore; }
    int getCurrentLine() const { return currentLine; }
    int getTotalLines() const { return totalLines; }
    const std::string& getCreationTime() const { return creationTime; }
    const std::string& getFinishTime() const { return finishTime; }
    void setFinishTime(std::string finTime) { finishTime= finTime; }
    void increaseCurrent(){ currentLine++; }
    bool isFinished() const { return fin; }
    void setFin(bool finish) { fin = fin; }
};
std::vector<Process*> process_queue;
std::vector<Process*> running_list;
std::vector<Process*> finished_list;
std::mutex mtx;

// Process Management
namespace ProcessManager {
    std::vector<Process> processes;

    void createProcess(const std::string& name) {
        Process* newProcess = new Process(name);
        processes.push_back(*newProcess);
        system("cls");
        newProcess->drawConsole();
    }

    Process* findProcess(const std::string& name) {
        for (auto& process : processes) {
            if (process.getName() == name) {
                return &process;
            }
        }
        return nullptr;
    }

    void redrawProcess(const std::string& name) {
        if (Process* process = findProcess(name)) {
            system("cls");
            process->drawConsole();
        }
        else {
            std::cout << "Process not found! Cannot redraw\n";
        }
    }

    void listProcess() {
        int cpu_utilization = (used_core * 100) / num_cpu;
        int cores_available = num_cpu - used_core;

        std::cout << "CPU Utilization: " << cpu_utilization << "%\n";
        std::cout << "Cores used: " << used_core << "\n";
        std::cout << "Cores available: " << cores_available << "\n";
        std::cout << "--------------------------------------\n";

        std::cout << "Running processes:\n";
        for (auto* process : running_list) {
            std::cout << process->getName() << "   " << "(" + Utils::getCurrentTimestamp() + ")"
                << "     core: " << process->getCore() << "    "
                << process->getCurrentLine() << "/" << process->getTotalLines() << "\n";
        }

        std::cout << "\nFinished processes:\n";
        for (auto* process : finished_list) {
            std::cout << process->getName() << "   " << process->getFinishTime()
                << "     Finished    " << process->getCurrentLine() << "/" << process->getTotalLines() << "\n";
        }

        std::cout << "--------------------------------------\n";
    }
}

// CPU worker function
void cpu_worker(int core_id) {
    bool running = true;
    while (running) {
        Process* process = nullptr;
        {
            std::lock_guard<std::mutex> lock(mtx);

            if (!process_queue.empty()) {
                process = process_queue.front();
                used_core++;
                running_list.emplace_back(process);
                process_queue.erase(std::remove(process_queue.begin(), process_queue.end(), process), process_queue.end());
            }
        }

        if (process) {
            std::string fileName = process->getName() + ".txt";
            std::remove(fileName.c_str());  // Delete the file if it exists

            std::ofstream file(process->getName() + ".txt", std::ios_base::app);
            file << "Process name: " << process->getName() + "\nLogs:\n" << std::endl;
            while (process->getCurrentLine() < process->getTotalLines()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                process->increaseCurrent();
                process->setFinishTime(Utils::getCurrentTimestamp());
                file << "(" << process->getFinishTime() << ") " << "Core:" << process->getCore() << " \"Hello world from " << process->getName() << "!\"" << std::endl;
            }            
            {
                std::unique_lock<std::mutex> lock(mtx);
                finished_list.emplace_back(process);
                running_list.erase(std::remove(running_list.begin(), running_list.end(), process), running_list.end());  // Remove from running list
                used_core--; 
                process->setFin(true);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (process_queue.empty() && running_list.empty()) {
            running = false;
        }
    }
}

// Scheduler function
void scheduler() {
    for (int i = 0; i < total_process; ++i) {
        Process* process = new Process("Process" + std::to_string(i + 1));
        process_queue.emplace_back(process);
        process->setCore(i % num_cpu);
    }

    std::vector<std::thread> cpu_thread;
    for (int i = 0; i < num_cpu; ++i) {
        cpu_thread.emplace_back(cpu_worker, i);
    }
    for (auto& th : cpu_thread) {
        th.join();
    }
}

// Command Handlers
void read_command(std::string input) {
    if (input == "initialize") {
        std::cout << "\n  initialize command recognized. Doing something...\n";
        system("cls");
    }
    else if (input.substr(0, 9) == "screen -s") {
        ProcessManager::createProcess(input.substr(10));
    }
    else if (input.substr(0, 9) == "screen -r") {
        ProcessManager::redrawProcess(input.substr(10));
    }
    else if (input == "screen -ls") {
        ProcessManager::listProcess();
    }
    else if (input == "scheduler-test") {
        std::cout << "scheduler-test command recognized. Doing something...\n";
    }
    else if (input == "scheduler-stop") {
        std::cout << "scheduler-stop command recognized. Doing something...\n";
    }
    else if (input == "report-util") {
        std::cout << "report-util command recognized. Doing something...\n";
    }
    else if (input == "clear") {
        system("cls");
        print_heading();
    }
    else {
        std::cout << "\n  '" << input << "' is not recognized as an internal or external command. Clearing menu...\n";
    }
}
