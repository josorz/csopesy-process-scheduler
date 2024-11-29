#include "MemoryManager.h"
#include <fstream>

MemoryManager* MemoryManager::sharedInstance = nullptr;

void MemoryManager::initialize(size_t memSize, size_t memPerFrame, bool isFlatAlloc) {
    if (!sharedInstance) {
        sharedInstance = new MemoryManager(memSize, memPerFrame, isFlatAlloc);
    }
}

MemoryManager* MemoryManager::getInstance() {
    return sharedInstance;
}

MemoryManager::MemoryManager(size_t memSize, size_t memPerFrame, bool isFlatAlloc)
    : maxOverallMem(memSize), memPerFrame(memPerFrame), isFlatAlloc(isFlatAlloc), numPagedIn(0), numPagedOut(0) {
    numFrames = calcFrames(memSize);
    for (size_t i = 0; i < numFrames; i++) {
        freeFrameList.push_back(i);
        allocationMap.push_back(std::make_pair(false, 0));       // Allocation map
    }
}

size_t MemoryManager::calcFrames(size_t size) const {
    return (size + memPerFrame - 1) / memPerFrame;
}

bool MemoryManager::allocateMem(Process& process) {
    size_t memoryRequired = process.getMemoryRequired();
  
    // Paging memory allocation
    size_t requiredPages = calcFrames(memoryRequired);

    if (requiredPages > freeFrameList.size()) {
        std::ofstream outFile("backing_store.txt", std::ios::app);

        if (requiredPages > freeFrameList.size()) {
            if (outFile.is_open()) {
                // Write the log message to the file
                outFile << "PID: " << process.getID() << " Name: " << process.getName() << " INS: " << process.getTotalLines() << " Num. Pages: " << requiredPages << " Mem. Req.: " << memoryRequired << ".\n";
            }
            else {
                std::cerr << "Failed to open backing store file!" << std::endl;
            }
            outFile.close();
            return false;
        }
        //std::cerr << "Cannot allocate. Not enough frames";
        return false;
    }

    for (size_t i = 0; i < requiredPages; i++) {
        size_t vacant_page = freeFrameList.front();

        // remove first element of frame list
        freeFrameList.erase(freeFrameList.begin());

        if (!allocationMap[vacant_page].first) {
            allocationMap[vacant_page] = { true, process.getID() };
            incrementPagedIn();
        }
        std::cout << "Allocating page " << vacant_page << " to process " << process.getID() << std::endl;
    }
    
    return true;
}

void MemoryManager::deallocate(int pid, size_t size) {
    for (size_t i = 0; i < numFrames; i++) {
        if (allocationMap[i].second == pid && allocationMap[i].first) {
            allocationMap[i].first = false;
            freeFrameList.push_back(i);
            incrementPagedOut();
        }
    }
}

const std::vector<std::pair<bool, int>>& MemoryManager::getAllocationMap() const {
    return allocationMap;
}

size_t MemoryManager::getUsedMemory() const {
    size_t usedMemory = 0;
    for (const auto& frame : allocationMap) {
        if (frame.first) { // Check if the frame is occupied
            usedMemory += memPerFrame;
        }
    }
    return usedMemory;
}