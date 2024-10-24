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
	virtual Process* findProcess(std::string name) {
		return nullptr;
	}
	void test() { this->isSchedulerOn = true; }
	void stop() { this->isSchedulerOn = false; }
	unsigned int delay_per_exec;
protected:
	std::vector<Process> finished_list;
	std::thread schedulerThread;
	int num_cpu;
	unsigned int quantum_cycles;
	unsigned int batch_process_freq;
	unsigned int min_ins;
	unsigned int max_ins;
	std::mutex m;
	bool isSchedulerOn = false;
};