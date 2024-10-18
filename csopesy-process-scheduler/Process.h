#pragma once
#include <iostream>

// Process Class
class Process {
private:
    std::string processName;
    int core;
    int currentLine;
    int totalLines;
    std::string creationTime;
    std::string finishTime;
    bool fin;

public:
    Process(std::string name);

    // Display process information on screen
    void drawConsole();

    const std::string& getName() const { return processName; }
    int getCore() const { return core; }
    void setCore(int newCore) { core = newCore; }
    int getCurrentLine() const { return currentLine; }
    int getTotalLines() const { return totalLines; }
    const std::string& getCreationTime() const { return creationTime; }
    const std::string& getFinishTime() const { return finishTime; }
    void setFinishTime(std::string finTime) { finishTime = finTime; }
    void increaseCurrent() { currentLine++; }
    bool isFinished() const { return currentLine == totalLines; }
};