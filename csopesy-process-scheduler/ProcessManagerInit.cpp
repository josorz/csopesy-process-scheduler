#include <fstream>
#include <sstream>

#include "ProcessManager.h"
#include "Scheduler.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"

void ProcessManager::init() {
    int num_cpu;
    unsigned int quantum_cycles, batch_process_freq, min_ins, max_ins, delay_per_exec;
    std::string schedulerType;
    
    std::ifstream file("config.txt");

    if (!file.is_open()) {
        std::cerr << "Error: config.txt cannot not found." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        if (iss >> key) {
            if (key == "num-cpu") {
                iss >> num_cpu;
                if (num_cpu > 128 || num_cpu < 1) {
                    std::cout << "Invalid bounds! Change initialize.txt";
                    return;
                }
            }
            else if (key == "scheduler") {
                iss >> std::ws;  // Skip whitespace
                std::getline(iss, schedulerType);  // Read the whole line (including quotes)
                // Remove the quotes from the string
                schedulerType = schedulerType.substr(1, schedulerType.length() - 2);
                if (!(schedulerType == "rr" || schedulerType == "fcfs")) {
                    std::cout << "Invalid bounds! Change initialize.txt";
                    return;
                }
            }
            else if (key == "quantum-cycles") {
                iss >> quantum_cycles;
                if (quantum_cycles < 1) {
                    std::cout << "Invalid bounds! Change initialize.txt";
                    return;
                }
            }
            else if (key == "batch-process-freq") {
                iss >> batch_process_freq;
                if (batch_process_freq < 1) {
                    std::cout << "Invalid bounds! Change initialize.txt";
                    return;
                }
            }
            else if (key == "min-ins") {
                iss >> min_ins;
                if (min_ins < 1) {
                    std::cout << "Invalid bounds! Change initialize.txt";
                    return;
                }
            }
            else if (key == "max-ins") {
                iss >> max_ins;
                if (max_ins < 1) {
                    std::cout << "Invalid bounds! Change initialize.txt";
                    return;
                }
            }
            else if (key == "delay-per-exec") {
                iss >> delay_per_exec;
            }
        }
    }

    file.close();

    std::cout << "CPU Cores: " << num_cpu << std::endl;
    std::cout << "Scheduler: " << schedulerType << std::endl;
    std::cout << "Quantum Cycles: " << quantum_cycles << std::endl;
    std::cout << "Batch Process Frequency: " << batch_process_freq << std::endl;
    std::cout << "Min Instructions: " << min_ins << std::endl;
    std::cout << "Max Instructions: " << max_ins << std::endl;
    std::cout << "Delay Per Exec: " << delay_per_exec << std::endl;

    if (schedulerType == "fcfs") {
        scheduler = new FCFSScheduler(num_cpu, batch_process_freq, min_ins, max_ins, delay_per_exec);
    }
    else if (schedulerType == "rr") {
        scheduler = new RRScheduler(num_cpu, quantum_cycles, batch_process_freq, min_ins, max_ins, delay_per_exec);
    }
    scheduler->init();
    std::thread start_scheduler(&Scheduler::run, scheduler);
    start_scheduler.detach();

    isInitialized = true;
}