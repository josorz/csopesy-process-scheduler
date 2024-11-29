#pragma once
#include <iostream>

// Process Class
class Process {
private:
    std::string processName;
    int id;
    int core;
    int currentLine;
    int totalLines;
    std::string creationTime;
    std::string finishTime;
    bool fin;

    static int processCounter;
    size_t memoryRequired;
    size_t numPages;
    bool isMemAllocated;

public:
    Process(std::string name, unsigned int min_ins, unsigned int max_ins, size_t minPerProc, size_t maxPerProc);
    // Display process information on screen
    void drawConsole();

    int getID() const { return id; }
    const std::string& getName() const { return processName; }
    int getCore() const { return core; }
    void setCore(int newCore) { core = newCore; }
    int getCurrentLine() const { return currentLine; }
    int getTotalLines() const { return totalLines; }
    const std::string getCreationTime() const { return creationTime; }
    const std::string getFinishTime() const { return finishTime; }
    void increaseCurrent() { currentLine++; }
    void setFinishTime();
    bool isFinished() const { return currentLine == totalLines; }

    size_t getMemoryRequired() const { return memoryRequired; };
    bool getMemAllocated() const { return isMemAllocated; }
    void setMemAllocated(bool status) { isMemAllocated = status; }

    size_t getNumPages() { return numPages; }
};