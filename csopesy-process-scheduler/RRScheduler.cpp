#include "RRScheduler.h"
#include "Process.h"
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>

RRScheduler::RRScheduler(int num_cpu, unsigned int quantum_cycles, unsigned int batch_process_freq, unsigned int min_ins, unsigned int max_ins, unsigned int delay_per_exec) : Scheduler() {
    this->num_cpu = num_cpu;
    this->quantum_cycles = quantum_cycles;
    this->batch_process_freq = batch_process_freq;
    this->min_ins = min_ins;
    this->max_ins = max_ins;
    this->delay_per_exec = delay_per_exec;
}

void RRScheduler::run() {
    unsigned int cpuCycles = 0;
    int processCtr = 0;
    std::string procName = "";
    while (true) {
        m.lock();
        if (!readyQueue.empty()) {
            // find vacant core
            for (auto& core : cores) {
                if (!core.isActive()) {
                    // remove process from queue
                    Process front = readyQueue.front();
                    readyQueue.pop_front();

                    // Assign process to the core
                    core.setProcess(front);

                    // Run the process for a time quantum
                    std::thread RRThread(&Core::runRRProcess, &core);
                    RRThread.detach();

                    break;
                }
            }
        }
        // scheduler-test
        if (isSchedulerOn && cpuCycles == batch_process_freq) {
            procName = procName + "process";
            if (processCtr < 10) {
                procName += "0";
            }
            procName += std::to_string(processCtr);

            readyQueue.push_back(Process(procName));
            procName = "";
            cpuCycles = 0;
            processCtr++;
        }
        m.unlock();

        if (cpuCycles == batch_process_freq)
            cpuCycles = 0;

        cpuCycles++;
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

    std::cout << "\nFinished processes:\n";
    for (auto process : finished_list) {
        std::cout << process.getName() << "   " << process.getFinishTime()
            << "     Finished    " << process.getCurrentLine() << "/" << process.getTotalLines() << "\n";
    }

    std::cout << "--------------------------------------\n";
}

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;
    localtime_s(&local_tm, &now_c);
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%m/%d/%Y, %I:%M:%S %p");
    return oss.str();
}

void RRScheduler::listProcessToFile() {
    std::ofstream reportFile("report.txt");  // Create a file named "report.txt"

    if (!reportFile.is_open()) {
        std::cerr << "Error opening report file.\n";
        return;
    }
    m.lock();

    reportFile << "Report generated at: " << getCurrentTimestamp() << "\n\n";
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


void RRScheduler::addProcess(Process p) {
    m.lock();
    readyQueue.push_back(p);
    m.unlock();
}

void RRScheduler::requeueProcess(Process &p) {
    m.lock();
    readyQueue.push_back(p);  // Reinsert process at the back of the queue
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

    return nullptr;
}