#pragma once
#include <iostream>
#include <memory>
#include "Process.h"

class Core
{
public:
	Core(int count, std::vector<Process*> &fin);
	std::string getProcessName();
	int getCore();
	int getCurrentLine();
	int getTotalLines();
	bool isActive();
	void setProcess(Process &p);
	void runProcess();
private:
	std::unique_ptr<Process> process = nullptr;
	std::vector<Process*> finished_list;
	int coreCount;
	bool active = false;
	bool isFinished();
};

