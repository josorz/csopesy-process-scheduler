#include <chrono>

#include "Core.h"
#include "FCFSScheduler.h"
#include "CPUTick.h"
#include "MemoryManager.h"
#include <mutex>

std::mutex z;

Core::Core(int count, Scheduler *s) {
	coreCount = count;
	scheduler = s;
	this->delay_per_exec = s->delay_per_exec;
}

Core::Core(int count, Scheduler* s, unsigned int quantum) {
	coreCount = count;
	scheduler = s;
	this->quantum = quantum;
	this->delay_per_exec = s->delay_per_exec;
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
		unsigned int local_cpu_ctr = CPUTick::getInstance().getTick();
		
		while (!p->isFinished()) {
			z.lock();

			unsigned int current_tick = CPUTick::getInstance().getTick();

			// Check if we have wrapped around
			if (current_tick >= local_cpu_ctr) {
				// Normal case
				if (current_tick - local_cpu_ctr >= delay_per_exec) {
					p->increaseCurrent();
					local_cpu_ctr = current_tick;  // Update local_cpu_ctr
				}
			}
			else {
				// Overflow case
				if ((UINT_MAX - local_cpu_ctr + current_tick + 1) >= delay_per_exec) {
					p->increaseCurrent();
					local_cpu_ctr = current_tick;  // Update local_cpu_ctr
				}
			}
			CPUTick::getInstance().addActiveTick();
			z.unlock();
		}

		z.lock();
		scheduler->finishProcess(*p);
		MemoryManager::getInstance()->deallocate(p->getID(), p->getMemoryRequired());
		process.reset();
		this->active = false;

		z.unlock();
	} else {
		CPUTick::getInstance().addIdleTick();
	}
}

void Core::runRRProcess() {
	if (process != nullptr && process.get() != nullptr) {
		this->active = true;

		Process* p = process.get();
		int timeRun = 0;  // Track time slice progress

		// Run for a time quantum or until finished
		unsigned int local_cpu_ctr = CPUTick::getInstance().getTick();

		while (!p->isFinished() && timeRun < quantum) {
			unsigned int current_tick = CPUTick::getInstance().getTick();

			// Check if we have wrapped around
			if (current_tick >= local_cpu_ctr) {
				// Normal case
				if (current_tick - local_cpu_ctr >= delay_per_exec) {
					p->increaseCurrent();
					local_cpu_ctr = current_tick;  // Update local_cpu_ctr
					timeRun++;
				}
			}
			else {
				// Overflow case
				if ((UINT_MAX - local_cpu_ctr + current_tick + 1) >= delay_per_exec) {
					p->increaseCurrent();
					local_cpu_ctr = current_tick;  // Update local_cpu_ctr
					timeRun++;
				}
			}
			CPUTick::getInstance().addActiveTick();
		}

		z.lock();

		if (p->isFinished()) {
			// If process finished, notify scheduler
			scheduler->finishProcess(*p);
			MemoryManager::getInstance()->deallocate(p->getID(), p->getMemoryRequired());
			process.reset();
		}
		else {
			// If process not finished, re-add to ready queue for next round
			scheduler->requeueProcess(*p);
			process.reset();
		}

		this->active = false;
		z.unlock();
	} else {
		CPUTick::getInstance().addIdleTick();
	}

}

Process* Core::getCurrentProcess() {
	return process.get();
}