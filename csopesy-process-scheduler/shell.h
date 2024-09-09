#pragma once

// TODO: replace if-else 
// https://chatgpt.com/share/fbf2eca5-47b1-42e7-b124-286805e30257

//void initialize();
//void screen(std::string arg1, std::string arg2);
//void scheduler_test();
//void scheduler_stop();
//void report_util();
//void clear();

void print_heading();

void read_command(std::string input) {
	if (input == "initialize") {
		std::cout << "initialize command recognized. Doing something\n";
		//initialize();
	}
	else if (input.substr(0, 6) == "screen") {
		std::cout << "screen command recognized. Doing something\n";
		// TODO: split to chunks to process
	}
	else if (input == "scheduler-test") {
		std::cout << "scheduler-test command recognized. Doing something\n";
		//scheduler_test();
	}
	else if (input == "scheduler-stop") {
		std::cout << "scheduler-stop command recognized. Doing something\n";
		//scheduler_stop();
	}
	else if (input == "report-util") {
		std::cout << "report-util command recognized. Doing something\n";
		//report_util();
	}
	else if (input == "clear") {
		system("cls");
		print_heading();
	}
	else {
		std::cout << "\'" << input << "\'" << " is not recognized as an internal or external command.\n";
	}
}