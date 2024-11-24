#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include "Process.h"
#include "Core.h"
#include <algorithm>

#include "ScheduleAlgo.h"

class Scheduler {
public:

	static void initialize(ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax, int numCores,
        double delayPerExec, unsigned minInstructions, unsigned maxInstruction, unsigned batchProcessFreq, size_t memPerProc);

    static Scheduler* getInstance();

    void run();
    void addNewProcess(std::shared_ptr<Process> process);
    void reAddProcess(std::shared_ptr<Process> process);
    std::shared_ptr<Process> getFirstProcess();
    void removeFirstProcess();
    void printProcesses();

    std::vector<std::shared_ptr<Process>>& getAllProcess();

    int getSize();
    int numCores();

    void schedulerTest();
    void schedulerStop();
    void schedulerTestLoop();
    void createProcess(int processID);
    int getAvailableCores();
    float getCPUUtilization();
    int getNumberOfCoresUsed();
    size_t getMemPerProc();

    void memoryReport(int counter);

	unsigned int getMinInstructions();
	unsigned int getMaxInstructions();

private:
    Scheduler(ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax, int numCores,
        double delayPerExec, unsigned minInstructions, unsigned maxInstruction, unsigned batchProcessFreq, size_t memPerProc);
    ~Scheduler();

    // Disable copying and assignment
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

    bool isRunning;
    ScheduleAlgo scheduleAlgo;
    unsigned int quantumCycleMax;
    unsigned int batchProcessFreq;      
    unsigned int minInstructions;       
    unsigned int maxInstructions;       
    double delayPerExec;
    size_t memPerProc = 0;
    bool schedulerTestFlag = false;
    std::condition_variable cv;

    std::vector<Core*> cores;

    static Scheduler* sharedInstance;
    std::thread workerThread;

    std::vector<std::shared_ptr<Process>> readyQueue;
    std::vector<std::shared_ptr<Process>> allProcesses;
    std::vector<std::thread> threads;

    std::thread testThread;
    int processCounter = 0;

    static std::mutex mtx;

    void firstComeFirstServe();
    void roundRobin();

    void sortReadyQueue();
};
