#pragma once
#include <deque>
#include <vector>
#include <thread>

#include "Scheduler.h"
#include "Process.h"
#include "Core.h"
class FCFSScheduler : public Scheduler
{
public:
	~FCFSScheduler();
	virtual void init();
	virtual void run();
	virtual void listProcess();
private:
	// https://stackoverflow.com/questions/5877504/access-c-queue-elements-like-an-array
	std::deque<Process> readyQueue;
	std::vector<Core> cores;
};