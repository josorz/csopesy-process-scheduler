#include "PrintCommand.h"
#include "iostream"

PrintCommand::PrintCommand(String& toPrint)
{
	this->toPrint = toPrint;
}

void PrintCommand::run()
{
	// std::cout << this->toPrint << std::endl;
}

String PrintCommand::getToPrint() {
	return this->toPrint;
}




