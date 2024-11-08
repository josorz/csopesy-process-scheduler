// refs:
// https://www.geeksforgeeks.org/first-fit-algorithm-in-memory-management-using-linked-list/
// https://www.geeksforgeeks.org/program-first-fit-algorithm-memory-management/?ref=rp

#include "MemoryManager.h"

#include <fstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <filesystem> 
#include <iostream>

// initialize memory manager with total memory and memory per process
FirstFitMemoryManager::FirstFitMemoryManager(int totalMem, int memPerProc)
    : totalMemory(totalMem), memPerProc(memPerProc) {
    freeList.emplace_back(0, totalMemory);  // Start with a single free block of total memory
}

// Allocate memory using the first-fit strategy
bool FirstFitMemoryManager::allocateMemory(const std::string& processName) {
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->size >= memPerProc) {
            // allocate memory from this block
            allocList.emplace_back(it->start, memPerProc, processName);
            std::cout << "Allocated " << memPerProc << " bytes to process " << processName << " at address " << it->start << "\n";
            it->start += memPerProc;
            it->size -= memPerProc;

            // remove block if fully allocated
            if (it->size == 0) {
                freeList.erase(it);
            }
            return true;
        }
    }
    //std::cout << "Not enough memory for process " << processName << "\n"; // debug
    return false;  // not enough memory available
}

// Free allocated memory for a specific process
void FirstFitMemoryManager::freeMemory(const std::string& processName) {
    for (auto it = allocList.begin(); it != allocList.end(); ++it) {
        if (it->processName == processName) {
            // Add the block back to the free list
            freeList.emplace_back(it->start, it->size);
            allocList.erase(it);
            mergeFreeBlocks();
            return;
        }
    }
}

// Merge adjacent free blocks to reduce fragmentation
void FirstFitMemoryManager::mergeFreeBlocks() {
    freeList.sort([](const FreeBlock& a, const FreeBlock& b) { return a.start < b.start; });
    for (auto it = freeList.begin(); it != freeList.end(); ) {
        auto next = std::next(it);
        if (next != freeList.end() && it->start + it->size == next->start) {
            it->size += next->size;
            freeList.erase(next);
        }
        else {
            ++it;
        }
    }
}

// Calculate total external fragmentation
int FirstFitMemoryManager::calculateExternalFragmentation() const {
    int fragmentation = 0;
    for (const auto& block : freeList) {
        fragmentation += block.size;
    }
    // used for debug
    //if (fragmentation != 16384 && fragmentation != 0) {
    //    std::cout << "Calculated External Fragmentation: " << fragmentation << " bytes\n";  
    //}
    return fragmentation;
}

// Function to generate a memory snapshot
void FirstFitMemoryManager::generateMemorySnapshot(int quantumCycle, const std::string& timestamp) {
    // store into folder 4 convenience
    const std::string folderPath = "snapshots";
    std::filesystem::create_directory(folderPath);

    // file path and open the file for writing
    std::string filePath = folderPath + "/memory_stamp_" + std::to_string(quantumCycle) + ".txt";
    std::ofstream file(filePath);

    // header 
    file << "Timestamp: " << timestamp << "\n";
    file << "Number of processes in memory: " << allocList.size() << "\n";

    int externalFragmentation = calculateExternalFragmentation();
    file << "Total external fragmentation in KB: " << externalFragmentation / 1024 << "\n";


    // printing memory layout
    file << "\n----end---- = " << totalMemory << "\n";
    for (const auto& block : allocList) {
        // each process upper limit, process name, and lower limit 
        file << block.start + block.size << "\n"
            << block.processName << "\n"
            << block.start << "\n";
    }
    file << "----start---- = 0\n";
}


// debug snapshot function
// Function to generate a memory snapshot
//void FirstFitMemoryManager::generateMemorySnapshot(int quantumCycle, const std::string& timestamp) {
//    const std::string folderPath = "snapshots2";
//    std::string filePath = folderPath + "/memory_stamp_" + std::to_string(quantumCycle) + ".txt";
//
//    int externalFragmentation = calculateExternalFragmentation();
//
//    if (externalFragmentation != 16384 && externalFragmentation != 0) {
//        std::cout << "Timestamp: " << timestamp << "\n";  // Assuming 'timestamp' is passed in as a parameter
//        std::cout << "Number of processes in memory: " << allocList.size() << "\n";
//        std::cout << "Total external fragmentation in KB: " << externalFragmentation / 1024 << "\n\n";
//
//        // Start memory layout
//        std::cout << "----end---- = " << totalMemory << "\n";
//        for (const auto& block : allocList) {
//            // each process upper limit, process name, and lower limit 
//            std::cout << block.start + block.size << "\n";
//            std::cout << block.processName << "\n";
//            std::cout << block.start << "\n";
//        }
//        std::cout << "----start---- = 0\n";
//    }
//}