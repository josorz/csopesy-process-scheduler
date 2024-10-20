#include "Scheduler.h"
#include <mutex>

std::mutex zz;

void Scheduler::init() {

}

void Scheduler::listProcess() {

}

void Scheduler::finishProcess(Process p) {
	zz.lock();
	p.setFinishTime();
	finished_list.push_back(p);
	zz.unlock();
}