#pragma once

#include <vector>
#include "Process.h"

class Scheduler
{
public:
	void init();
	void listProcess();
private:
	int num_cpu;
	int batch_process_freq;
	int min_ins;
	int max_ins;
	int delays_per_exec;

	friend class Core;
};