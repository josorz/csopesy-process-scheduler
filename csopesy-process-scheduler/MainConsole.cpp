#pragma once
#include "MainConsole.h"
#include "TypedefRepo.h"
#include "ConsoleManager.h"
#include "Process.h"
#include "AConsole.h"
#include <iostream>
#include "utils.h"
#include <chrono>
#include <thread>
#include <sstream>
#include <memory>
#include <iomanip>
#include "PrintCommand.h"
#include "Scheduler.h"
#include <fstream>
#include <stdexcept>
#include <random>

extern ConsoleManager consoleManager;

MainConsole::MainConsole() : AConsole("MAIN_CONSOLE")
{
}

void MainConsole::display() {
    displayHeader();
    process();
}

void MainConsole::onEnabled()
{
    displayHeader();
}

void MainConsole::process() {
    String command;
    while (true) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, command);
        handleCommand(command);
        std::cout << std::endl;
    }
}

void MainConsole::displayHeader() {
    Utils::printHeader();
}

std::pair<String, String> parseScreenCommand(String userInput) {
    String command;
    String name;
    std::stringstream ss(userInput);
    ss >> command;  // Gets 'screen'
    if (ss >> command && (command == "-r" || command == "-s")) {
        ss >> name;  // Gets the <name>
    }
    return { command, name };
}

void MainConsole::handleCommand(String command)
{
    auto consoleManager = ConsoleManager::getInstance();
    if (command == "exit")
    {
        std::cout << "Exiting the program..." << std::endl;
        exit(0);
        std::terminate();

    }    
    else if (command == "clear" || command == "cls")
    {
        system("cls");
        displayHeader();
    }

    else if (command.substr(0, 6) == "screen")
    {
        auto [screenCommand, processName] = parseScreenCommand(command);

        if (screenCommand.empty())
        {
            std::cout << "Incomplete arguments. Use 'screen -s <name>' or 'screen -r <name>'." << '\n';

        }

        else if (screenCommand == "-ls")
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before modifying the vector
            std::cout << "CPU Utilization: "
                << std::fixed << std::setprecision(2)
                << Scheduler::getInstance()->getCPUUtilization() * 100  
                << "%" << '\n';
			std::cout << "Cores used: " << Scheduler::getInstance()->getNumberOfCoresUsed() << '\n';
			std::cout << "Cores available: " << Scheduler::getInstance()->getAvailableCores() << '\n';
            std::cout << '\n';
            std::cout << "---------------------------------------" << '\n';
            std::vector<std::shared_ptr<Process>>& allProcesses = Scheduler::getInstance()->getAllProcess();

            if (allProcesses.empty())
            {
                std::cout << "You currently don't have any processes." << '\n';
            }
            else
            {
                std::cout << "Running Processes:\n";
            	for (const std::shared_ptr<Process>& process : allProcesses)
                {
                    process->printInfo();
                    //if (process->getCoreID() != -1 && process->getState() != Process::FINISHED)
                    //{
                    //    std::cout << std::left << std::setw(25) << process->getName()  
                    //        << std::setw(30) << process->getFormattedTime()
                    //        << "Core:" << process->getCoreID() << '\t'
                    //        << '\t' << process->getCurrentInstruction() << "/"
                    //        << process->getTotalInstructions() << '\n';
                    //}
                }
                
                std::cout << "\nFinished Processes:\n";

                for (const std::shared_ptr<Process>& process : allProcesses)
                {
                    if (process->getState() == Process::FINISHED) 
                    {
                        std::cout << std::left << process->getName() << " | "
                            << process->getFormattedTime() << " | "
                            << "Status: FINISHED | "
                            << process->getCurrentInstruction() << " / "
                            << process->getTotalInstructions() << " |" << '\n';
                    }
                }
            }
            std::cout << "---------------------------------------" << '\n';
        }


        else if (processName.empty())
        {
            std::cout << "Incomplete arguments. Use 'screen -s <name>' or 'screen -r <name>'." << '\n';
        }
        else if (screenCommand == "-s")
        {
            if (!consoleManager->isScreenRegistered(processName))
            {
                std::random_device rd;  // Random number seed
                std::mt19937 gen(rd()); // Random number generator (Mersenne Twister)
                std::uniform_int_distribution<> dis(Scheduler::getInstance()->getMinInstructions(), Scheduler::getInstance()->getMaxInstructions());

                int totalinstructions = dis(gen);
                // Register new process and switch to the screen
                String toPrint = "Hello world from " + processName;
                auto process = std::make_shared<Process>(processName, Scheduler::getInstance()->getAllProcess().size(), totalinstructions, PrintCommand(toPrint), Scheduler::getInstance()->getMemPerProc());
                auto processScreen = std::make_shared<BaseScreen>(process, processName);

                Scheduler::getInstance()->addNewProcess(process);

                consoleManager->registerScreen(processScreen);
                consoleManager->switchToScreen(processName);
            }
            else
            {
                std::cout << "Process '" << processName << "' already exists or has existed. Please provide a different name." << std::endl;
            }
        }
        else if (screenCommand == "-r")
        {
            if (!consoleManager->isScreenRegistered(processName))
            {
                std::cout << "Process '" << processName << "' not found." << std::endl;
            }
            else
            {
                consoleManager->switchToScreen(processName);
            }
        }
        else
        {
            std::cout << "Incomplete arguments. Use 'screen -s <name>' or 'screen -r <name>'." << '\n';
        }
    }

    else if (command == "scheduler-test")
    {
		Scheduler::getInstance()->schedulerTest();
        std::cout << "Scheduler test ongoing. Continuously generating dummy processes.\n";
    }
    else if (command == "scheduler-stop")
    {
        Scheduler::getInstance()->schedulerStop();
        std::cout << "Stopped generating dummy processes.\n";
    }
    else if (command == "report-util")
    {
        saveReport();
    }
    else if (command == "debug-info")
    {
        std::cout << "Size of Ready Queue: " << Scheduler::getInstance()->getSize() << '\n';
        std::cout << "Number of cores: " << Scheduler::getInstance()->numCores() << '\n';
        std::cout << "Process List: \n";
        Scheduler::getInstance()->printProcesses();
    }
    else if (command == "initialize")
    {
        std::cout << "The emulator has already been initialized." << std::endl;
    }
    else
    {
        std::cout << "Invalid command. Please try again." << std::endl;
    }
}

String MainConsole::formatNA(int num)
{
	if (num == -1)
	{
        return "N/A";
	}

	return std::to_string(num);

}

void MainConsole::saveReport()
{
    std::ofstream file("csopesy-log.txt");
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    file << "CPU Utilization: "
        << std::fixed << std::setprecision(2)
        << Scheduler::getInstance()->getCPUUtilization() * 100
        << "%" << '\n';
    file << "Cores used: " << Scheduler::getInstance()->getNumberOfCoresUsed() << '\n';
    file << "Cores available: " << Scheduler::getInstance()->getAvailableCores() << '\n';
    file << '\n';
    file << "---------------------------------------" << '\n';
    std::vector<std::shared_ptr<Process>>& allProcesses = Scheduler::getInstance()->getAllProcess();

    if (allProcesses.empty())
    {
        file << "You currently don't have any processes." << '\n';
    }
    else
    {
        file << "Running Processes:\n";

        for (const std::shared_ptr<Process>& process : allProcesses)
        {
            if (process->getCoreID() != -1 && process->getState() != Process::FINISHED)
            {
                file << process->getName() << " | "
                    << process->getFormattedTime() << " | "
                    << "Core: " << process->getCoreID() << " | "
                    << "Status: Running | "
                    << process->getCurrentInstruction() << " / "
                    << process->getTotalInstructions() << " |" << '\n';
            }
        }

        file << "\nFinished Processes:\n";

        for (const std::shared_ptr<Process>& process : allProcesses)
        {
            if (process->getState() == Process::FINISHED)
            {
                file << process->getName() << " | "
                    << process->getFormattedTime() << " | "
                    << "Status: FINISHED | "
                    << process->getCurrentInstruction() << " / "
                    << process->getTotalInstructions() << " |" << '\n';
            }
        }
    }
    file << "---------------------------------------" << '\n';
    std::cout << "Report generated at csopesy-log.txt!" << std::endl;
}