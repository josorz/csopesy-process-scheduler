#include "RRScheduler.h"
#include "MemoryManager.h"
#include "Process.h"
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>

#include "CPUTick.h"

int processCtr = 0;
std::string procName = "";

RRScheduler::RRScheduler(int num_cpu, unsigned int quantum_cycles, unsigned int batch_process_freq, unsigned int min_ins, unsigned int max_ins, unsigned int delay_per_exec, size_t minPerProc, size_t maxPerProc) : Scheduler() {
    this->num_cpu = num_cpu;
    this->quantum_cycles = quantum_cycles;
    this->batch_process_freq = batch_process_freq;
    this->min_ins = min_ins;
    this->max_ins = max_ins;
    this->delay_per_exec = delay_per_exec;
    this->minPerProc = minPerProc;
    this->maxPerProc = maxPerProc;
}

void RRScheduler::run() {
    while (true) {
        m.lock();
        // Check if there are processes in either the readyQueue or rrQueue
        for (auto& core : cores) {
            if (!core.isActive()) {  // If core is inactive, we can assign a new process

                // Check for a process in the readyQueue first
                if (!readyQueue.empty()) {
                    // Remove process from the readyQueue
                    Process front = readyQueue.front();
                    readyQueue.pop_front();

                    // Assign process to the core
                    core.setProcess(front);

                    // Run the process for a time quantum
                    std::thread RRThread(&Core::runRRProcess, &core);
                    RRThread.detach(); // Detach the thread to allow it to run independently

                    break; // Exit after assigning a process to one core
                }

                // If no process was taken from readyQueue, check rrQueue
                if (!rrQueue.empty()) {
                    // Remove process from the rrQueue
                    Process front = rrQueue.front();
                    rrQueue.pop_front();

                    // Assign process to the core
                    core.setProcess(front);

                    // Run the process for a time quantum
                    std::thread RRThread(&Core::runRRProcess, &core);
                    RRThread.detach(); // Detach the thread to allow it to run independently

                    break; // Exit after assigning a process to one core
                } 
            } else {
                CPUTick::getInstance().addIdleTick();
            }
        }
        scheduler_test();
        m.unlock();
        CPUTick::getInstance().addTick();
    }
}


RRScheduler::~RRScheduler() {
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void RRScheduler::init() {

    // initialize cores
    for (int x = 0; x < num_cpu; x++) {
        cores.push_back(Core(x, this, quantum_cycles));
    }

    // run scheduler thread 
    schedulerThread = std::thread(&RRScheduler::run, this);
}

void RRScheduler::listProcess() {
    m.lock();

    int used_core = 0;
    for (auto& core : cores) {
        if (core.isActive()) {
            used_core++;
        }
    }

    double cpu_utilization = (used_core * 100) / this->num_cpu;
    int cores_available = this->num_cpu - used_core;

    std::cout << "CPU Utilization: " << cpu_utilization << "%\n";
    std::cout << "Cores used: " << used_core << "\n";
    std::cout << "Cores available: " << cores_available << "\n";
    std::cout << "--------------------------------------\n";

    std::cout << "Running processes:\n";
    for (auto& core : cores) {
        if (core.isActive()) {
            std::cout << core.getProcessName() << "   " << "(" << core.getCreationTime() << ")"
                << "     core: " << core.getCore() << "    "
                << core.getCurrentLine() << "/" << core.getTotalLines() << "\n";
        }
    }
    m.unlock();

    std::ostringstream oss;
    oss << "Finished processes:\n";
    for (const auto& process : finished_list) {
        oss << process.getName() << "   " << process.getFinishTime()
            << "     Finished     " << process.getCurrentLine() << "/" << process.getTotalLines() << "\n";
    }
    std::cout << oss.str();
    std::cout << "--------------------------------------\n";
}

void RRScheduler::listProcessToFile() {
    std::ofstream reportFile("report.txt");  // Create a file named "report.txt"

    if (!reportFile.is_open()) {
        std::cerr << "Error opening report file.\n";
        return;
    }
    m.lock();

    reportFile << "Report generated on: " << getCurrentTimestamp() << "\n\n";
    reportFile << "--------------------------------------\n\n";

    int used_core = 0;

    for (auto& core : cores) {
        if (core.isActive()) {
            used_core++;
        }
    }

    double cpu_utilization = (used_core * 100) / this->num_cpu;
    int cores_available = this->num_cpu - used_core;

    reportFile << "CPU Utilization: " << cpu_utilization << "%\n";
    reportFile << "Cores used: " << used_core << "\n";
    reportFile << "Cores available: " << cores_available << "\n";
    reportFile << "--------------------------------------\n";

    reportFile << "Running processes:\n";
    for (auto& core : cores) {
        if (core.isActive()) {
            reportFile << core.getProcessName() << "   " << "(" << core.getCreationTime() << ")"
                << "     core: " << core.getCore() << "    "
                << core.getCurrentLine() << "/" << core.getTotalLines() << "\n";
        }
    }
    m.unlock();

    reportFile << "\nFinished processes:\n";
    for (auto process : finished_list) {
        reportFile << process.getName() << "   " << process.getFinishTime()
            << "     Finished    " << process.getCurrentLine() << "/" << process.getTotalLines() << "\n";
    }

    reportFile << "--------------------------------------\n";
    reportFile.close();

    std::cout << "Report saved to report.txt\n";
}

void RRScheduler::processSMI() {
    m.lock();
    MemoryManager* memManager = MemoryManager::getInstance();

    // Get memory usage
    size_t totalMemory = memManager->getMemorySize();
    size_t usedMemory = memManager->getUsedMemory();
    double memoryUtil = (static_cast<double>(usedMemory) / totalMemory) * 100;

    // Get CPU utilization
    int usedCores = 0;
    for (auto& core : cores) {
        if (core.isActive()) {
            usedCores++;
        }
    }
    double cpuUtilization = (static_cast<double>(usedCores) / num_cpu) * 100;

    // Display the header
    std::cout << "-----------------------------------------------\n";
    std::cout << "| PROCESS-SMI V01.00 Driver Version: 01.00 |\n";
    std::cout << "-----------------------------------------------\n";
    std::cout << "CPU-Util: " << cpuUtilization << "%\n";
    std::cout << "Memory Usage: " << usedMemory / 1024 << "MiB / " << totalMemory / 1024 << "MiB\n";
    std::cout << "Memory Util: " << memoryUtil << "%\n";
    std::cout << "===============================================\n";
    std::cout << "Running Processes and Memory Usage:\n";
    std::cout << "-----------------------------------------------\n";

    // Display running processes and their memory usage
    for (auto& core : cores) {
        if (core.isActive()) {
            Process* currentProcess = core.getCurrentProcess();
            if (currentProcess != nullptr) {
                std::cout << currentProcess->getName() << "  "
                    << currentProcess->getMemoryRequired() << "MiB\n";
            }
        }
    }

    std::cout << "-----------------------------------------------\n";

    m.unlock();
}

void RRScheduler::vmstat() {
    m.lock();
    MemoryManager* memManager = MemoryManager::getInstance();
    if (!memManager) {
        std::cerr << "Memory Manager is not initialized.\n";
    }

    // Memory Statistics
    size_t totalMemoryKB = memManager->getMemorySize(); // Total memory in KB
    size_t usedMemoryKB = memManager->getUsedMemory();  // Used memory in KB
    size_t freeMemoryKB = totalMemoryKB - usedMemoryKB; // Free memory in KB

    // CPU Statistics
    unsigned int idleTicks = CPUTick::getInstance().getIdleTicks();
    unsigned int activeTicks = CPUTick::getInstance().getActiveTicks();
    unsigned int totalTicks = activeTicks + idleTicks;

    // Paging Statistics
    size_t numPagedIn = memManager->getPagedInCount();   // Number of pages paged in
    size_t numPagedOut = memManager->getPagedOutCount(); // Number of pages paged out

    // Output the statistics
    std::cout << "===============================================\n";
    std::cout << "Total memory: " << totalMemoryKB << " KB\n";
    std::cout << "Used memory: " << usedMemoryKB / 1024 << " MiB\n";
    std::cout << "Free memory: " << freeMemoryKB / 1024 << " MiB\n";
    std::cout << "Idle cpu ticks: " << idleTicks << " Ticks\n";
    std::cout << "Active cpu ticks: " << activeTicks << " Ticks\n";
    std::cout << "Total cpu ticks: " << totalTicks << " Ticks\n";
    std::cout << "Num paged in: " << numPagedIn << " ins\n";
    std::cout << "Num paged out: " << numPagedOut << " outs\n";
    std::cout << "-----------------------------------------------\n";
    
    m.unlock();
}

void RRScheduler::addProcess(Process p) {
    m.lock();
    readyQueue.push_back(p);
    m.unlock();
}

void RRScheduler::requeueProcess(Process &p) {
    m.lock();
    rrQueue.push_back(p);  // Reinsert process at the back of the queue
    m.unlock();
}

Process *RRScheduler::findProcess(std::string name) {
    // get processes from ready queue
    for (auto& process : readyQueue) {
        if (process.getName() == name) {
            return &process;
        }
    }

    // get processes from cores
    for (auto& core : cores) {
        if (core.isActive()) {
            if (core.getCurrentProcess()->getName() == name) {
                Process *p = core.getCurrentProcess();
                return p;
            }
        }
    }

    // Get finished processes
    for (const auto& process : finished_list) {
        if (process.getName() == name) {
            return const_cast<Process*>(&process); // Cast away constness to return a Process pointer
        }
    }

    return nullptr;
}

void RRScheduler::stop() {
    m.lock();
    isSchedulerOn = false;
    readyQueue = std::deque<Process>();
    m.unlock();
}

void RRScheduler::scheduler_test() {
    unsigned int tick = CPUTick::getInstance().getTick();

    if (!isSchedulerOn) return;

    // scheduler-test
    if (isSchedulerOn && tick % batch_process_freq == 0) {
        procName = "process";

        if (processCtr < 10) {
            procName += "0" + std::to_string(processCtr);
        }
        else {
            procName += std::to_string(processCtr);
        }

        readyQueue.push_back(Process(procName, min_ins, max_ins, minPerProc, maxPerProc));

        procName = "";
        processCtr++;
    }
}