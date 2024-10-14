#pragma once
#include <iostream>
#include "ProcessManager.h"

class MainConsole {
public:
	MainConsole();
	~MainConsole() = default;
	void printHeading();
	void drawConsole();
	void readCommand(std::string input);
private:
	bool isExitRaised = false;
	ProcessManager processManager = ProcessManager();
};