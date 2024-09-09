#include <fstream>
#include <sstream>

struct Config {
    int num_cpu;
    std::string scheduler;
    int quantum_cycles;
    int batch_process_freq;
    int min_ins;
    int max_ins;
    double delay_per_exec;
};

bool check_bounds(int number, int lb, int ub) {
    return number >= lb && number <= ub;
}

bool initialize(Config *config) {
    // DISCLAIMER: this snippet is straight outta CHATGPT
    // https://chatgpt.com/share/24b3ad0b-f442-4201-adcb-5f77645806b2
    std::ifstream file("config.txt");

    if (!file.is_open()) {
        std::cerr << "Error: config.txt cannot not found." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        if (iss >> key) {
            if (key == "num-cpu") {
                iss >> config->num_cpu;
            }
            else if (key == "scheduler") {
                iss >> std::ws;  // Skip whitespace
                std::getline(iss, config->scheduler);  // Read the whole line (including quotes)
                // Remove the quotes from the string
                config->scheduler = config->scheduler.substr(1, config->scheduler.length() - 2);
            }
            else if (key == "quantum-cycles") {
                iss >> config->quantum_cycles;
            }
            else if (key == "batch-process-freq") {
                iss >> config->batch_process_freq;
            }
            else if (key == "min-ins") {
                iss >> config->min_ins;
            }
            else if (key == "max-ins") {
                iss >> config->max_ins;
            }
            else if (key == "delay-per-exec") {
                iss >> config->delay_per_exec;
            }
        }
    }

    file.close();

    // end snippet

    // Validate if outputs are correct
    check_bounds(config->num_cpu, 1, 128);

    check_bounds(config->quantum_cycles, 1, 4294967296);
    check_bounds(config->batch_process_freq, 1, 4294967296);
    check_bounds(config->min_ins, 1, 4294967296);
    check_bounds(config->max_ins, 1, 4294967296);
    check_bounds(config->delay_per_exec, 0, 4294967296);

    // Output to verify the values
    std::cout << "CPU Cores: " << config->num_cpu << std::endl;
    std::cout << "Scheduler: " << config->scheduler << std::endl;
    std::cout << "Quantum Cycles: " << config->quantum_cycles << std::endl;
    std::cout << "Batch Process Frequency: " << config->batch_process_freq << std::endl;
    std::cout << "Min Instructions: " << config->min_ins << std::endl;
    std::cout << "Max Instructions: " << config->max_ins << std::endl;
    std::cout << "Delay Per Exec: " << config->delay_per_exec << std::endl;

    return true;
}
