#include "BaseScreen.h"
#include "ConsoleManager.h"
#include <iostream>
#include "utils.h"

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName)
{
	this->attachedProcess = process;

	auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm timeInfo;
	localtime_s(&timeInfo, &currentTime);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%m/%d/%Y %I:%M:%S%p", &timeInfo);
	this->screenCreationTime = buffer;
}

void BaseScreen::onEnabled()
{
	process();
}

void BaseScreen::process()
{
	if (this->refreshed == false)
	{
		this->refreshed = true;
		this->printProcessInfo();
	}

	std::string command;
	while (command != "exit")
	{
		std::cout << "Enter a command: ";
		std::getline(std::cin, command);

		if (command == "process-smi")
		{
			this->printProcessInfo();
		}
		else if (command == "clear" || command == "cls")
		{
			system("cls");
			this->printProcessInfo();
		}
		else if (command == "exit")
		{
			this->refreshed = false;
			ConsoleManager::getInstance()->returnToPreviousConsole();
		}
		else
		{
			std::cout << Colors::Red << "Command not found." << Colors::White << std::endl << std::endl;
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

void BaseScreen::display()
{
	this->printProcessInfo();
}

void BaseScreen::printProcessInfo()
{
	std::cout << "Process: " << this->attachedProcess->getName() << std::endl;
	std::cout << "ID: " << std::to_string(this->attachedProcess->getID()) << std::endl;
	std::cout << std::endl;
	if (this->attachedProcess->getState() != Process::FINISHED)
	{
		std::cout << "Current Instruction Line: " + std::to_string(this->attachedProcess->getCurrentInstruction()) << std::endl;
		std::cout << "Lines of code: " + std::to_string(this->attachedProcess->getTotalInstructions()) << std::endl;
		std::cout << "Screen Created At: " + this->screenCreationTime << std::endl;
	}
	else
	{
		std::cout << "Finished." << std::endl;
		std::cout << "Screen Created At: " + this->screenCreationTime << std::endl;
	}
}