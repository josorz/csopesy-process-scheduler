#pragma once
#include "TypedefRepo.h"

class PrintCommand
{
public:
	PrintCommand(String& toPrint);
	void run();
	String getToPrint();
private:
	String toPrint;
};

