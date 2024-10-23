#pragma once
#include "Scheduler.h"
#include "Core.h"

class RRScheduler : public Scheduler
{
public:
	RRScheduler(int num_cpu, unsigned int batch_process_freq, unsigned int min_ins, unsigned int max_ins, unsigned int delay_per_exec);
	~RRScheduler();
	void init() override;
	void run() override;
	void listProcess() override;
	void addProcess(Process p) override;
private:
	std::vector<Process> readyQueue;
	std::vector<Core> cores;
};

