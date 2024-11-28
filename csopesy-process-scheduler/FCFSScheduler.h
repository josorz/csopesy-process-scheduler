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
	FCFSScheduler(int num_cpu, unsigned int batch_process_freq, unsigned int min_ins, unsigned int max_ins, unsigned int delay_per_exec, size_t minPerProc, size_t maxPerProc);
	~FCFSScheduler();
	void init() override;
	void run() override;
	void listProcess() override;
	void listProcessToFile() override;
	void addProcess(Process p) override;
	void stop() override;
	Process* findProcess(std::string name) override;
	void scheduler_test();
private:
	// https://stackoverflow.com/questions/5877504/access-c-queue-elements-like-an-array
	std::deque<Process> readyQueue;
	std::vector<Core> cores;
	unsigned int cpuCycles = 0;
	int processCtr = 0;
	std::string procName = "";
	size_t minPerProc;
	size_t maxPerProc;
};