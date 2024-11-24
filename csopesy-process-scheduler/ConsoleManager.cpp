#include "ConsoleManager.h"
#include <iostream>
#include "MainConsole.h"
#include "TypedefRepo.h"
#include "utils.h"

ConsoleManager* ConsoleManager::sharedInstance = nullptr;
ConsoleManager* ConsoleManager::getInstance()
{
	return sharedInstance;
}

void ConsoleManager::initialize()
{
	sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy()
{
	delete sharedInstance;
}

ConsoleManager::~ConsoleManager()
{
}

void ConsoleManager::drawConsole() const
{
	if (this->currentConsole != nullptr)
	{
		this->currentConsole->display();
	}
	else
	{
		std::cerr << "No console assigned." << std::endl;
	}
}

void ConsoleManager::process() const
{
	if (this->currentConsole != nullptr)
	{
		this->currentConsole->process();
	}
	else
	{
		std::cerr << "No console assigned." << std::endl;
	}
}

void ConsoleManager::switchConsole(String consoleName)
{
	std::cout << "Switching to console: " << consoleName << '\n';
	if (this->consoleTable.contains(consoleName))
	{
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cerr << "Console not found." << std::endl;
	}
}

void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef)
{
	if (this->consoleTable.contains(screenRef->getName()))
	{
		std::cerr << "Screen already registered." << std::endl;
		return;
	}
	this->consoleTable[screenRef->getName()] = screenRef;
}

void ConsoleManager::switchToScreen(String screenName)
{
	if (this->consoleTable.contains(screenName))
	{
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[screenName];
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cerr << "Screen not found." << std::endl;
	}
}

void ConsoleManager::unregisterScreen(String screenName)
{
	if (this->consoleTable.contains(screenName))
	{
		this->consoleTable.erase(screenName);
	}
	else
	{
		std::cerr << "Screen not found." << std::endl;
	}
}

ConsoleManager::ConsoleManager()
{
	this->running = true;
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();
	this->consoleTable[MAIN_CONSOLE] = mainConsole;

	this->switchConsole(MAIN_CONSOLE);
}

bool ConsoleManager::isScreenRegistered(const String& screenName) {
	return consoleTable.find(screenName) != consoleTable.end();
}

void ConsoleManager::returnToPreviousConsole() {
	if (previousConsole) {
		currentConsole = previousConsole;
		previousConsole = nullptr;
		system("cls");
		currentConsole->display();
	}
	else {
		std::cerr << "No previous screen to return to." << std::endl;
	}
}

bool ConsoleManager::isRunning() const
{
	return this->running;
}