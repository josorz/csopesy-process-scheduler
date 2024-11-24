#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "utils.h"

void Utils::printConfirmation(const string& command) {
    cout << Colors::Green << "\n  " << command << " command recognized. Doing something. . ." << Colors::White;
    Sleep(1000);
    system("cls");
    Utils::printHeader();
}

void Utils::printHeader() {
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