#include <iostream>
#include <chrono>
#include <ctime>
#include "Process.h"
#include <thread>
#include <mutex>
// Utility Functions

int Process::processCounter = 0;

namespace Utils {
    // used for now
    int generateRandomNumber(int lower, int upper) {
        srand(time(NULL));
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
    totalLines = rand() % (max_ins - min_ins + 1) + min_ins;
    creationTime = Utils::getCurrentTimestamp();
    fin = false;

    id = ++processCounter;
}

// display process information on screen
void Process::drawConsole() {

    std::string command = "process-smi";
    while (command == "process-smi") {
        system("cls");
        std::cout << "\n  --- SCREEN: " << " ---\n";
        std::cout << "  Process: " << getName() << "\n";
        std::cout << "  ID: " << getID() << "\n\n"; // change this to process ID
        if (getCurrentLine() >= getTotalLines()) {
            std::cout << "  Finished!\n";
        }
        else {
            std::cout << "  Current Instruction line: " << getCurrentLine() << "\n";
            std::cout << "  Lines of code: " << getTotalLines() << "\n";
        }
        std::cout << "  ---------------------------\n";

        if (getCurrentLine() >= getTotalLines()) {
            std::cout << "  Finished!\n";
        }

        std::cout << "\n  Type 'process-smi' to view more information on this process: ";
        std::cout << "\n  Type 'exit' to return back to main menu: ";
        std::getline(std::cin, command);
        if (command != "exit" && command != "process-smi") {
            std::cout << "\n  Invalid input. Press enter to return to main menu: \n";
            std::cin.ignore();
        }
    }
    system("cls");
}

void Process::setFinishTime() { 
    finishTime = Utils::getCurrentTimestamp(); 
}