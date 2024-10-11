#include<iostream>
#include<string>

#include"shell.h"

void read_command(std::string input);
void scheduler();

void print_heading() {
    std::cout << "  _____   _____   ____   _____   ______   _____ __     __\n";
    std::cout << " / ____| / ____| / __ \\ |  __ \\ |  ____| / ____|\\ \\   / /\n";
    std::cout << "| |     | (___  | |  | || |__) || |__   | (___   \\ \\_/ / \n";
    std::cout << "| |      \\___ \\ | |  | ||  ___/ |  __|   \\___ \\   \\   /  \n";
    std::cout << "| |____  ____) || |__| || |     | |____  ____) |   | |   \n";
    std::cout << " \\_____||_____/  \\____/ |_|     |______||_____/    |_|   \n";
    std::cout << "\033[32mHello, Welcome to CSOPESY commandline!\n"; // \033[32m = green color
    std::cout << "\033[93mType \'exit\' to quit, \'clear\' to clear the screen\n";
    std::cout << "\033[0m";
}

// dummy config for scheduler
std::string schedulerAlgo = "fcfs";
int used_core = 0;
int num_cpu = 4;
int total_process = 10;

int main() {
    print_heading();
    //sheduler-test
    std::thread scheduler_thread(scheduler);

    while (true) {
        // Get user input
        std::string input;
        std::cout << "Enter a command: ";
        std::getline(std::cin, input);

        // Handle exit explicitly
        if (input == "exit") {
            std::cout << "Exiting the program...\n";
            break;
        }

        // Pass the command to read_command function (shell.h)
        read_command(input);
    }

    scheduler_thread.join();

    return 0;
}
