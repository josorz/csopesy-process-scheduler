#include <iostream>
#include <chrono>
#include <ctime>
#include "Process.h"

// Utility Functions
namespace Utils {
    // used for now
    int generateRandomNumber(int lower, int upper) {
        srand(time(0));
        return rand() % (upper - lower + 1) + lower;
    }

    // get current timestamp as a string
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm;
        localtime_s(&local_tm, &now_c);
        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%m/%d/%Y, %I:%M:%S %p");
        return oss.str();
    }
}

// initializes process with a name and random instruction count
Process::Process(std::string name, unsigned int min_ins, unsigned int max_ins) {
    processName = name;
    core = -1;
    currentLine = 0;
    totalLines = Utils::generateRandomNumber(min_ins, max_ins);
    creationTime = Utils::getCurrentTimestamp();
    fin = false;
}

// display process information on screen
void Process::drawConsole() {
    // TODO: replace with proper getline
    std::string command;
    while (command != "exit") {
        system("cls");
        std::cout << "\n  --- SCREEN: " << this->processName << " ---\n";
        std::cout << "  Process name: " << this->processName << "\n";
        std::cout << "  Current Instruction: " << this->currentLine << '/' 
            << this->totalLines << "\n";
        std::cout << "  Created At: " << this->creationTime << "\n";
        std::cout << "  ---------------------------\n";

        std::cout << "\n  Type 'exit' to return back to main menu: ";
        std::getline(std::cin, command);
    }
    system("cls");
}

void Process::setFinishTime() { 
    finishTime = Utils::getCurrentTimestamp(); 
}