#include <chrono>

#include "Core.h"
#include "FCFSScheduler.h"

#include <mutex>

std::mutex z;

Core::Core(int count, Scheduler *s) {
	coreCount = count;
	scheduler = s;
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

std::string Core::getCreationTime() {
	return process.get()->getCreationTime();
}

bool Core::isActive() {
	//return active;
	return process != nullptr;
}

void Core::setProcess(Process &p) {
	process = std::make_unique<Process>(p);	
}

void Core::runProcess() {
	if (process != nullptr && process.get() != nullptr) {
		this->active = true;

		Process* p = process.get();

		while (!p->isFinished()) {
			p->increaseCurrent();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		z.lock();
		scheduler->finishProcess(*p);

		process.reset();
		this->active = false;

		z.unlock();
	}
}