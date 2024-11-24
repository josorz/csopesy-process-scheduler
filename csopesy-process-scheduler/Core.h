#pragma once
#include "Process.h"
#include <thread>

#include "ScheduleAlgo.h"

class Core
{
public:
	Core(double delayPerExec, int coreID, ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax);
	void attachProcess(std::shared_ptr<Process> process);
	std::shared_ptr<Process> getAttachedProcess();
	void runFCFS();
	void runRR();

	bool hasAttachedProcess();

	bool finishedQuantumCycle();
	void resetQuantumCycle();

	int getCoreID();

	void resetTickDelay();

	void detachProcess();

private:
	std::mutex mtx;

	double delayPerExec;
	unsigned int currentTickDelay;
	int coreID;

	void incrementTickDelay();

	unsigned int quantumCycle;
	unsigned int quantumCycleMax;
	std::shared_ptr<Process> attachedProcess;
	std::thread workerThread;
};

