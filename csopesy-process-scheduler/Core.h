#pragma once
#include <iostream>
#include <memory>
#include "Process.h"
#include "Scheduler.h"

class Core
{
public:
	Core(int count, Scheduler *s);
	Core(int count, Scheduler* s, unsigned int quantum);
	std::string getProcessName();
	std::string getCreationTime();
	int getCore();
	int getCurrentLine();
	int getTotalLines();
	bool isActive();
	void setProcess(Process &p);
	void runProcess();
	// For RR Sched
	void runRRProcess();
	void requeue(Process& p);
	Process* getCurrentProcess();
private:
	std::unique_ptr<Process> process = nullptr;
	Scheduler *scheduler;
	int coreCount;
	int quantum;
	bool active = false;
	bool isFinished();
	unsigned int delay_per_exec;

};

