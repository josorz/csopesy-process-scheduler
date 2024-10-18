#pragma once
#include <iostream>
#include "ProcessManager.h"
#include "FCFSScheduler.h"

class MainConsole {
public:
	MainConsole();
	~MainConsole() = default;
	void printHeading();
	void drawConsole();
	void readCommand(std::string input);
	bool isInitialized();
private:
	bool isExitRaised = false;
	bool initialized = false;
	ProcessManager processManager = ProcessManager();
	FCFSScheduler temp;
};