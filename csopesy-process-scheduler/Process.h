#pragma once
#include <iostream>

// Process Class
class Process {
private:
    std::string processName;
    int currentLine;
    int totalLines;
    std::string creationTime;
    
public:
    // initializes process with a name and random instruction count
    Process(std::string name);

    // display process information on screen
    void drawConsole();
    const std::string& getName() const { return processName; };
};