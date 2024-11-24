#pragma once
#include "BaseScreen.h"
#include "TypedefRepo.h"

class MainConsole : public AConsole
{
public:
	MainConsole();
	~MainConsole() = default;

	void onEnabled();
	void display();
	void process();

	String formatNA(int num);

	void saveReport();

private:
	void displayHeader();
	void handleCommand(String command);

	std::mutex mtx;
};

