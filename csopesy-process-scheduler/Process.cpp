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

    // screen loop func
    void runScreenLoop(std::function<void()> action) {
        std::string command;
        while (command != "exit") {
            system("cls");
            action();
            std::cout << "\n  Type 'exit' to return back to main menu: ";
            std::getline(std::cin, command);
        }
        system("cls");
    }
}

// initializes process with a name and random instruction count
Process::Process(std::string name) {
    processName = name;
    totalLines = Utils::generateRandomNumber(100, 200);
    currentLine = totalLines - Utils::generateRandomNumber(20, 50);
    creationTime = Utils::getCurrentTimestamp();
}

// display process information on screen
void Process::drawConsole() {
    auto showDetails = [&]() {
        std::cout << "\n  --- SCREEN: " << processName << " ---\n";
        std::cout << "  Process name: " << processName << "\n";
        std::cout << "  Current Instruction: " << currentLine << '/' << totalLines << "\n";
        std::cout << "  Created At: " << creationTime << "\n";
        std::cout << "  ---------------------------\n";
        };
    Utils::runScreenLoop(showDetails);
}

const std::string& Process::getName() const { return processName; }