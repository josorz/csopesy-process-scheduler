#include "FCFSScheduler.h"
#include "Process.h"
#include <thread>
#include <mutex>


FCFSScheduler::FCFSScheduler(int num_cpu, unsigned int batch_process_freq, unsigned int min_ins, unsigned int max_ins, unsigned int delay_per_exec) : Scheduler() {
    this->num_cpu = num_cpu;
    this->batch_process_freq = batch_process_freq;
    this->min_ins = min_ins;
    this->max_ins = max_ins;
    this->delay_per_exec = delay_per_exec;
}

void FCFSScheduler::run() {
    while (true) {
        m.lock();
        if (!readyQueue.empty()) {
            // find vacant core
            for (auto& core : cores) {
                if (!core.isActive()) {

                    // remove
                    Process front = readyQueue.front();
                    core.setProcess(front);
                    // Run

                    std::thread FCFSThread(&Core::runProcess, &core);
                    FCFSThread.detach();

                    readyQueue.pop_front();

                    break;
                }
            }
        }
        m.unlock();
    }
}

FCFSScheduler::~FCFSScheduler() {
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void FCFSScheduler::init() {
    int num_cores = 4;

    // initialize cores
    for (int x = 0; x < num_cores; x++) {
        cores.push_back(Core(x, this));
    }

    // run scheduler thread 
    schedulerThread = std::thread(&FCFSScheduler::run, this);
}

void FCFSScheduler::listProcess() {
    int used_core = 4;
    int num_cores = 4;
    int cpu_utilization = (used_core * 100) / num_cores;
    int cores_available = num_cores - used_core;

    std::cout << "CPU Utilization: " << cpu_utilization << "%\n";
    std::cout << "Cores used: " << used_core << "\n";
    std::cout << "Cores available: " << cores_available << "\n";
    std::cout << "--------------------------------------\n";

    std::cout << "Running processes:\n";
    for (auto& core : cores) {
        if (core.isActive()) {
            std::cout << core.getProcessName() << "   " << "(" + core.getCreationTime()
                << "     core: " << core.getCore() << "    "
                << core.getCurrentLine() << "/" << core.getTotalLines() << "\n";
        }
    }

    std::cout << "\nFinished processes:\n";
    for (auto process : finished_list) {
        std::cout << process.getName() << "   " << process.getFinishTime()
            << "     Finished    " << process.getCurrentLine() << "/" << process.getTotalLines() << "\n";
    }

    std::cout << "--------------------------------------\n";
}

void FCFSScheduler::addProcess(Process p) {
    m.lock();
    readyQueue.push_back(p);
    m.unlock();
}