#include "FCFSScheduler.h"
#include "Process.h"
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <deque>

#include "CPUTick.h"

#include "MemoryManager.h"

FCFSScheduler::FCFSScheduler(int num_cpu, unsigned int batch_process_freq, unsigned int min_ins, unsigned int max_ins, unsigned int delay_per_exec, size_t minPerProc, size_t maxPerProc) : Scheduler() {
    this->num_cpu = num_cpu;
    this->batch_process_freq = batch_process_freq;
    this->min_ins = min_ins;
    this->max_ins = max_ins;
    this->delay_per_exec = delay_per_exec;
    this->minPerProc = minPerProc;
    this->maxPerProc = maxPerProc;
}

void FCFSScheduler::run() {
    int processCtr = 0;
    while (true) {
        
        m.lock();
        if (!readyQueue.empty()) {
            // find vacant core
            for (auto& core : cores) {
                if (!core.isActive()) {
                    // remove
                    Process front = readyQueue.front();
                    core.setProcess(front);

                    // check if process can run given the current memory
                    if (MemoryManager::getInstance()->getFreeFrames() >= front.getNumPages()) {
                        // Run
                        std::cerr << "Running process " << front.getName() << " \n";

                        std::thread FCFSThread(&Core::runProcess, &core);
                        FCFSThread.detach();

                        readyQueue.pop_front();
                    }
                    else {
                        std::cerr << "Cannot run process " << front.getName() << " due to memory constraints\n";

                        // Move to the back (???)
                        readyQueue.pop_front();
                        readyQueue.push_back(front);
                    }

                    break;
                }
            }
        }
        
        m.unlock();

        m.lock();
        scheduler_test();
        m.unlock();

        CPUTick::getInstance().addTick();
    }
}

void FCFSScheduler::scheduler_test() {
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
        cpuCycles = 0;
        processCtr++;
    }
}

FCFSScheduler::~FCFSScheduler() {
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void FCFSScheduler::init() {

    // initialize cores
    for (int x = 0; x < num_cpu; x++) {
        cores.push_back(Core(x, this));
    }

    // run scheduler thread 
    schedulerThread = std::thread(&FCFSScheduler::run, this);
}

void FCFSScheduler::listProcess() {
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
            << "     Finished     " << process.getCurrentLine() << "/" << 
process.getTotalLines() << "\n";
    }
    std::cout << oss.str();
    std::cout << "--------------------------------------\n";
}

void FCFSScheduler::listProcessToFile() {
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
void FCFSScheduler::addProcess(Process p) {
    m.lock();
    readyQueue.push_back(p);
    m.unlock();
}

Process* FCFSScheduler::findProcess(std::string name) {
    // Get processes from ready queue
    for (auto& process : readyQueue) {
        if (process.getName() == name) {
            return &process;
        }
    }

    // Get processes from cores
    for (auto& core : cores) {
        if (core.isActive()) {
            if (core.getCurrentProcess()->getName() == name) {
                return core.getCurrentProcess();
            }
        }
    }

    // Get finished processes
    for (const auto& process : finished_list) {
        if (process.getName() == name) {
            return const_cast<Process*>(&process); // Cast away constness to return a Process pointer
        }
    }

    return nullptr; // Return nullptr if no matching process found
}

void FCFSScheduler::stop() {
    isSchedulerOn = false;

    readyQueue = std::deque<Process>();
    processCtr = finished_list.size();
}