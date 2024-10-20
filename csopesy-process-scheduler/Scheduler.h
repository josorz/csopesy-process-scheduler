#pragma once

#include <thread>
#include <vector>
#include "Process.h"

class Scheduler
{
public:
	void init();
	void finishProcess(Process p);
	void listProcess();
private:
	int num_cpu;
	int batch_process_freq;
	int min_ins;
	int max_ins;
	int delays_per_exec;
protected:
	std::vector<Process> finished_list;
	std::thread schedulerThread;
};