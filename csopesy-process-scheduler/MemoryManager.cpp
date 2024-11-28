#include "MemoryManager.h"

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
    : maxOverallMem(memSize), memPerFrame(memPerFrame) {
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
        std::cerr << "Cannot allocate. Not enough frames";
        return false;
    }

    for (size_t i = 0; i < requiredPages; i++) {
        size_t vacant_page = freeFrameList.front();

        // remove first element of frame list
        freeFrameList.erase(freeFrameList.begin());

        if (!allocationMap[vacant_page].first) {
            allocationMap[vacant_page] = { true, process.getID() };
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
        }
    }
}

const std::vector<std::pair<bool, int>>& MemoryManager::getAllocationMap() const {
    return allocationMap;
}

size_t MemoryManager::getMemorySize() const {
    return maxOverallMem;
}

size_t MemoryManager::getMemPerFrame() const {
    return memPerFrame;
}

size_t MemoryManager::getNumFrames() const {
    return numFrames;
}