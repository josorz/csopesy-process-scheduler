#pragma once
#include <vector>
#include "Process.h"
#include "Scheduler.h"

// Process Management
class ProcessManager {
private:
    Scheduler * scheduler;
    std::vector<Process> processes;
    bool isInitialized = false;
public:
    ~ProcessManager();
    // func to create new process and display its console
    void createProcess(const std::string& name);
    
    void init();
    void listProcess();

    // find a process by name
    Process* findProcess(const std::string& name);

    // redraw console for an existing process
    void redrawProcess(const std::string& name);
};