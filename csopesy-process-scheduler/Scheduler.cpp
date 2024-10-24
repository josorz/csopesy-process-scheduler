#include "Scheduler.h"
#include <mutex>

std::mutex zz;

void Scheduler::init() {
	std::cout << "test";
}

void Scheduler::run() {

}

void Scheduler::listProcess() {

}

void Scheduler::addProcess(Process p) {
}

void Scheduler::finishProcess(Process p) {
	zz.lock();
	p.setFinishTime();
	finished_list.push_back(p);
	zz.unlock();
}