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
	FCFSScheduler(int num_cpu, unsigned int batch_process_freq, unsigned int min_ins, unsigned int max_ins, unsigned int delay_per_exec);
	~FCFSScheduler();
	void init() override;
	virtual void run();
	void listProcess() override;
	void addProcess(Process p) override;
private:
	// https://stackoverflow.com/questions/5877504/access-c-queue-elements-like-an-array
	std::deque<Process> readyQueue;
	std::vector<Core> cores;
};