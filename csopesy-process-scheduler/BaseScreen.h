#pragma once
#include "AConsole.h"
#include "TypedefRepo.h"
#include "Process.h"
#include <memory>

class BaseScreen : public AConsole
{
public:
	BaseScreen(std::shared_ptr<Process> process, String processName);

	void onEnabled() override;
	void display() override;
	void process() override;

private:
	void printProcessInfo();
	std::shared_ptr<Process> attachedProcess;
	String screenCreationTime;
	bool refreshed = false;
};

