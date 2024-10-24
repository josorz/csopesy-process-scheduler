#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include "Process.h"

class Scheduler
{
public:
	virtual ~Scheduler() {}
	virtual void init();
	virtual void run();
	void finishProcess(Process p);
	virtual void listProcess();
	virtual void addProcess(Process p);
	virtual void requeueProcess(Process& p) {}
protected:
	std::vector<Process> finished_list;
	std::thread schedulerThread;
	int num_cpu;
	unsigned int quantum_cycles;
	unsigned int batch_process_freq;
	unsigned int min_ins;
	unsigned int max_ins;
	unsigned int delay_per_exec;
	std::mutex m;
};