#include "Core.h"
#include "Scheduler.h"
#include "MemoryManager.h"

Core::Core(double delayPerExec, int coreID, ScheduleAlgo scheduleAlgo, unsigned int quantumCycleMax)
{
	this->delayPerExec = delayPerExec;
	this->coreID = coreID;
	this->quantumCycle = 0;
	this->quantumCycleMax = quantumCycleMax;
	this->attachedProcess = nullptr;
	//this->attachedProcess = nullptr;

	// Start the thread in the constructor
	if (scheduleAlgo == FCFS)
	{
		this->workerThread = std::thread(&Core::runFCFS, this);
	}
	else if (scheduleAlgo == RR)
	{
		this->workerThread = std::thread(&Core::runRR, this);
	}

	this->workerThread.detach();
}

void Core::attachProcess(std::shared_ptr<Process> process)
{
	std::lock_guard<std::mutex> lock(this->mtx);
	this->attachedProcess = process;
}

void Core::detachProcess()
{
	this->attachedProcess = nullptr;
}


std::shared_ptr<Process> Core::getAttachedProcess()
{
	return this->attachedProcess;
}

void Core::runFCFS()
{
	while(true)
	{
		this->mtx.lock();
		if (this->hasAttachedProcess())
		{
			if (this->attachedProcess->getState() != Process::FINISHED)
			{
				this->attachedProcess->run();
			}
			else if (this->attachedProcess->getState() == Process::FINISHED)
			{
				detachProcess();
			}
		}
		this->mtx.unlock();
		std::this_thread::sleep_for(std::chrono::duration<double>(this->delayPerExec));
	}

}

void Core::runRR()
{
	while (true)
	{
		this->mtx.lock();
		if (this->hasAttachedProcess())
		{
			if (!this->finishedQuantumCycle()
				&& this->attachedProcess->getState() == Process::RUNNING)
			{
				this->attachedProcess->run();
				this->quantumCycle = this->quantumCycle + 1;
			}
			else if (this->attachedProcess->getState() == Process::FINISHED)
			{
				MemoryManager::getInstance()->deallocate(attachedProcess->getID(), attachedProcess->getMemoryRequired());
				attachedProcess->setInMemory(false);
				detachProcess();
			}
		}
		
		this->mtx.unlock();
		std::this_thread::sleep_for(std::chrono::duration<double>(this->delayPerExec));
	}
}

void Core::resetQuantumCycle()
{
	std::lock_guard<std::mutex> lock(this->mtx);
	this->quantumCycle = 0;
}

bool Core::finishedQuantumCycle()
{
	if (this->quantumCycle >= this->quantumCycleMax)
	{
		return true;
	}

	return false;
	//return this->quantumCycle >= this->quantumCycleMax;
}


void Core::resetTickDelay()
{
	this->currentTickDelay = 0;
}

void Core::incrementTickDelay()
{
	if (this->currentTickDelay >= this->delayPerExec)
	{
		resetTickDelay();
		return;
	}

	this->currentTickDelay = this->currentTickDelay + 1;
}



bool Core::hasAttachedProcess()
{
	//std::lock_guard<std::mutex> lock(this->mtx);
	if (this->attachedProcess)
	{
		return true;
	}

	return false;
}

int Core::getCoreID()
{
	return this->coreID;
}
