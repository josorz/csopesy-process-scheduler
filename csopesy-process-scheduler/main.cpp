#include<iostream>
#include<string>
#include"initialize.h"


bool initialize(Config *config);

void printHeading() {
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

int main() {
	printHeading();
	bool isInitialized;
	// Get User Input
	std::string input;
	Config config;
	while (true) {
		std::cout << "Enter a command: ";
		std::getline(std::cin, input);
		if (input == "exit") {
			break;
		}
	};

	return 0;
}