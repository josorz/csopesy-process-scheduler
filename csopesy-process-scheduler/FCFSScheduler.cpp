#include "FCFSScheduler.h"
#include "Process.h"
#include <thread>
#include <mutex>

std::mutex m;

void FCFSScheduler::run() {
    while (true) {
        if (!readyQueue.empty()) {
            // find vacant core
            m.lock();
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
            m.unlock();
        }
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

    readyQueue.push_back(Process("one"));
    readyQueue.push_back(Process("two"));
    readyQueue.push_back(Process("three"));
    readyQueue.push_back(Process("four"));
    readyQueue.push_back(Process("five"));
    readyQueue.push_back(Process("six"));
    readyQueue.push_back(Process("seven"));
    readyQueue.push_back(Process("eight"));
    readyQueue.push_back(Process("nine"));
    readyQueue.push_back(Process("ten"));

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