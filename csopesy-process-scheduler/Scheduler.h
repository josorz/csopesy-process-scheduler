#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include "Process.h"
#include "MemoryManager.h"

class Scheduler
{
public:
    virtual ~Scheduler() {}
    virtual void init();
    virtual void run();

    // Modified finishProcess to free memory for a completed process
    void finishProcess(Process p);

    // Modified startProcess to attempt memory allocation before starting a process
    virtual bool startProcess(Process& p) {
        if (memoryManager.allocateMemory(p.getName())) {
            // If memory allocation is successful, proceed with starting the process
            return true;
        }
        else {
            // If memory allocation fails, requeue the process
            requeueProcess(p);
            return false;
        }
    }

    virtual void listProcess();
    virtual void listProcessToFile();
    virtual void addProcess(Process p);
    virtual void requeueProcess(Process& p) {}
    virtual Process* findProcess(std::string name) {
        return nullptr;
    }
    void test() { this->isSchedulerOn = true; }
    virtual void stop() {};
    unsigned int delay_per_exec;
    std::string getCurrentTimestamp();

protected:
    std::vector<Process> finished_list;
    std::thread schedulerThread;
    int num_cpu;
    unsigned int quantum_cycles;
    unsigned int batch_process_freq;
    unsigned int min_ins;
    unsigned int max_ins;
    std::mutex m;
    bool isSchedulerOn = false;

    // Add memory manager instance with total memory 16384 bytes and memory per process 4096 bytes
    FirstFitMemoryManager memoryManager{ 16384, 4096 };
};
