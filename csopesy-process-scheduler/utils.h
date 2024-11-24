#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <random>
#include <sstream>
#include <windows.h>
#include <fstream>

using namespace std;

typedef std::string String;

namespace Colors {
    const string Yellow = "\033[0;33m";
    const string Green = "\033[0;32m";
    const string Red = "\033[0;31m";
    const string White = "\033[0m";
}

namespace Utils {
    void printConfirmation(const string& command);
    void printHeader();
}
