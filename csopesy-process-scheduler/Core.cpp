#include <chrono>

#include "Core.h"
#include "FCFSScheduler.h"
#include "Scheduler.h"

Core::Core(int count, std::vector<Process*> &fin) {
	coreCount = count;
	finished_list = fin;
}

int Core::getCore() {
	return coreCount;
}

int Core::getCurrentLine() {
	return process.get()->getCurrentLine();
}

int Core::getTotalLines() {
	return process.get()->getTotalLines();
}

std::string Core::getProcessName() {
	return process.get()->getName();
}

bool Core::isActive() {
	//return active;
	return process != nullptr;
}

void Core::setProcess(Process &p) {
	process = std::make_unique<Process>(p);	
}

void Core::runProcess() {
	if (process.get() != nullptr) {
		this->active = true;

		while (!process.get()->isFinished()) {
			process.get()->increaseCurrent();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		Process* p = process.get();

		std::cout << process.get()->getName();

		finished_list.push_back(p);

		process = nullptr;
		this->active = false;
	}
}