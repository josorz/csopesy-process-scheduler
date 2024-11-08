#include "Scheduler.h"
#include <mutex>
#include <ctime>
#include <chrono>
#include <sstream>

std::mutex zz;

void Scheduler::init() {
	std::cout << "test";
}

void Scheduler::run() {

}

void Scheduler::listProcess() {

}

void Scheduler::listProcessToFile() {

}

void Scheduler::addProcess(Process p) {
}

void Scheduler::finishProcess(Process p) {
    std::lock_guard<std::mutex> lock(zz);  
    p.setFinishTime();
    finished_list.push_back(p);
    memoryManager.freeMemory(p.getName());  // Free memory for the completed process

    // debug
    //std::cout << "Process " << p.getName() << " finished and added to finished_list.\n";
}

std::string Scheduler::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;
    localtime_s(&local_tm, &now_c);
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%m/%d/%Y, %I:%M:%S %p");
    return oss.str();
}