#include "RRScheduler.h"
#include "Process.h"
#include <thread>
#include <mutex>
#include <vector>

RRScheduler::RRScheduler(int num_cpu, unsigned int quantum_cycles, unsigned int batch_process_freq, unsigned int min_ins, unsigned int max_ins, unsigned int delay_per_exec) : Scheduler() {
    this->num_cpu = num_cpu;
    this->quantum_cycles = quantum_cycles;
    this->batch_process_freq = batch_process_freq;
    this->min_ins = min_ins;
    this->max_ins = max_ins;
    this->delay_per_exec = delay_per_exec;
}

void RRScheduler::run() {
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
        m.unlock();
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

    readyQueue.push_back(Process("process01"));
    readyQueue.push_back(Process("process02"));
    readyQueue.push_back(Process("process03"));
    readyQueue.push_back(Process("process04"));
    readyQueue.push_back(Process("process05"));
    readyQueue.push_back(Process("process06"));
    readyQueue.push_back(Process("process07"));
    readyQueue.push_back(Process("process08"));
    readyQueue.push_back(Process("process09"));
    readyQueue.push_back(Process("process10"));


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