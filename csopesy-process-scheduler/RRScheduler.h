#pragma once
#include <deque>

#include "Scheduler.h"
#include "Core.h"


class RRScheduler : public Scheduler
{
public:
	RRScheduler(int num_cpu, unsigned int quantum_cycles, unsigned int batch_process_freq, unsigned int min_ins, unsigned int max_ins, unsigned int delay_per_exec);
	~RRScheduler();
	void init() override;
	void run() override;
	void listProcess() override;
	void listProcessToFile() override;
	void addProcess(Process p) override;
	void requeueProcess(Process &p) override;
	Process *findProcess(std::string name) override;
private:
	std::deque<Process> readyQueue;
	std::vector<Core> cores;
};

