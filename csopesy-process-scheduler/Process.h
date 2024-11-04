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

    static int processCounter; // consider changing, static/global var = bad

public:
    Process(std::string name, unsigned int min_ins, unsigned int max_ins);

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
};