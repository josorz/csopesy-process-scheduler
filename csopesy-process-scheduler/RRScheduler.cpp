#include "RRScheduler.h"
#include "Process.h"
#include <thread>
#include <mutex>


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
    int num_cores = 4;

    // initialize cores
    for (int x = 0; x < num_cores; x++) {
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