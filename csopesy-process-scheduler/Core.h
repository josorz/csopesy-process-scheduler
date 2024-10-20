#pragma once
#include <iostream>
#include <memory>
#include "Process.h"
#include "Scheduler.h"

class Core
{
public:
	Core(int count, Scheduler *s);
	std::string getProcessName();
	std::string getCreationTime();
	int getCore();
	int getCurrentLine();
	int getTotalLines();
	bool isActive();
	void setProcess(Process &p);
	void runProcess();
private:
	std::unique_ptr<Process> process = nullptr;
	Scheduler *scheduler;
	int coreCount;
	bool active = false;
	bool isFinished();

};

