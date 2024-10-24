#include <chrono>

#include "Core.h"
#include "FCFSScheduler.h"

#include <mutex>

std::mutex z;

Core::Core(int count, Scheduler *s) {
	coreCount = count;
	scheduler = s;
}

Core::Core(int count, Scheduler* s, unsigned int quantum) {
	coreCount = count;
	scheduler = s;
	this->quantum = quantum;
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

void Core::runRRProcess() {
	if (process != nullptr && process.get() != nullptr) {
		this->active = true;

		Process* p = process.get();
		int timeRun = 0;  // Track time slice progress

		// Run for a time quantum or until finished
		while (!p->isFinished() && timeRun < quantum) {
			p->increaseCurrent();  // Process does work
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			timeRun += 1;  // Update time run
		}

		z.lock();

		if (p->isFinished()) {
			// If process finished, notify scheduler
			scheduler->finishProcess(*p);
			process.reset();
		}
		else {
			// If process not finished, re-add to ready queue for next round
			scheduler->addProcess(*p);
			process.reset();
		}

		this->active = false;
		z.unlock();
	}

}

Process* Core::getCurrentProcess() {
	return process.get();
}