#include "Process.h"
#include <iostream>
#include <filesystem> 
#include "ConsoleManager.h"
#include "PrintCommand.h"

Process::Process(String processName, int id, int totalInstructions ,PrintCommand command, size_t memoryRequired) : creationTime(std::chrono::system_clock::now()), command(command)
{
	this->processName = processName;
	this->processState = READY;
	this->id = id;
	this->inCoreID = -1;
	this->command = command;
	this->totalInstructions = totalInstructions;
	this->currentInstruction = 0;

	this->memoryRequired = memoryRequired;
	this->inMemory = false;

	//this->openLogFile();
}

void Process::run()
{
	std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
	// print something
	this->command.run();
	//this->logPrintCommand(this->command.getToPrint());

	this->currentInstruction = currentInstruction + 1;
	if (currentInstruction >= totalInstructions)
	{
		this->processState = FINISHED;
		//ConsoleManager::getInstance()->unregisterScreen(this->processName);
		//this->closeLogFile();
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Process::setCoreID(int coreID)
{
	//std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
	this->inCoreID = coreID;
}

int Process::getCoreID()
{
	//std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
	return this->inCoreID;
}

int Process::getID()
{
	return this->id;
}

std::string Process::getName() const {
	return processName;
}

int Process::getCurrentInstruction() {
	return currentInstruction;
}

int Process::getTotalInstructions() {
	return totalInstructions;
}

std::chrono::system_clock::time_point Process::getCreationTime() const
{
	return this->creationTime;
}

Process::state Process::getState()
{
	std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
	return this->processState;
}

void Process::runningState()
{
	this->processState = RUNNING;
}

void Process::readyState()
{
	this->processState = READY;
}

void Process::finishState()
{
	this->processState = FINISHED;
}

size_t Process::getMemoryRequired()
{
	return this->memoryRequired;
}

bool Process::isInMemory()
{
	return this->inMemory;
}

void Process::setInMemory(bool inMemory)
{
	this->inMemory = inMemory;
}



void Process::printInfo()
{
	//std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
	if (this->getCoreID() != -1 && this->processState != Process::FINISHED)
	{
		std::cout << std::left << std::setw(25) << this->getName() << " | "
			<< std::setw(30) << this->getFormattedTime() << " | "
			<< "Core: " << this->getCoreID() << " | "
			<< this->getCurrentInstruction() << " / "
			<< this->getTotalInstructions() << " |" << '\n';

	}
}

String Process::getFormattedTime() {
	auto creationTimeT = std::chrono::system_clock::to_time_t(this->creationTime);
	struct tm timeinfo;
	localtime_s(&timeinfo, &creationTimeT);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "(%m/%d/%Y %I:%M:%S%p)", &timeinfo);
	return buffer;
}

void Process::openLogFile() {

	const std::string directory = "Process Text"; // directory file

	if (!std::filesystem::exists(directory)) {
		std::filesystem::create_directory(directory);
	}


	std::string filename = directory + "/" + processName + ".txt";


	logFile.open(filename);
	if (!logFile.is_open()) {
		std::cerr << "Failed to open log file for Process " << id << ".\n";
	}
	else {
		logFile << "Process: " << processName << "\nLogs:\n\n";
	}
}

void Process::logPrintCommand(const std::string& command) {
	std::time_t timestamp = std::time(nullptr);
	std::tm timeInfo;
	localtime_s(&timeInfo, &timestamp);
	char timeBuffer[25];
	std::strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y %I:%M:%S%p", &timeInfo);
	logFile << "(" << timeBuffer << ") Core: " << inCoreID << " \"" << command << "\"\n";
}

void Process::closeLogFile() {
	logFile.close();
}