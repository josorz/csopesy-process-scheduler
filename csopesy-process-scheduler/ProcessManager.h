#pragma once
#include <vector>
#include "Process.h"

// Process Management
class ProcessManager {
private:
    std::vector<Process> processes;

public:
    ~ProcessManager() = default;
    // func to create new process and display its console
    void createProcess(const std::string& name);

    // find a process by name
    Process* findProcess(const std::string& name);

    // redraw console for an existing process
    void redrawProcess(const std::string& name);
};