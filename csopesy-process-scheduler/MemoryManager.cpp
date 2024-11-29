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
    : maxOverallMem(memSize), memPerFrame(memPerFrame), isFlatAlloc(isFlatAlloc), numPagedIn(0), numPagedOut(0) {
    numFrames = calcFrames(memSize);
    if (isFlatAlloc) {
        // Initialize memory for flat allocation
        memory = std::vector<char>(memSize, '.');
    }
    allocationMap.resize(numFrames, { false, 0 }); // {isAllocated, processID}
}

size_t MemoryManager::calcFrames(size_t size) const {
    return (size + memPerFrame - 1) / memPerFrame;
}

bool MemoryManager::canAllocateFlat(size_t size, size_t* frameIndex) {
    bool allocated = false;
    size_t requiredFrames = calcFrames(size);

    for (size_t mainPointer = 0; mainPointer < numFrames; mainPointer++) {
        allocated = true;
        for (size_t tracker = mainPointer; tracker < mainPointer + requiredFrames; tracker++) {
            if (tracker >= numFrames || allocationMap[tracker].first) {
                mainPointer = tracker; // Skip ahead to the next unallocated region
                allocated = false;
                break;
            }
        }
        if (allocated) {
            *frameIndex = mainPointer;
            break;
        }
    }
    return allocated;
}

bool MemoryManager::allocateMem(Process& process) {
    size_t frameIndex;
    size_t memoryRequired = process.getMemoryRequired();

    if (isFlatAlloc) {
        // Flat memory allocation
        if (canAllocateFlat(memoryRequired, &frameIndex)) {
            size_t requiredFrames = calcFrames(memoryRequired);
            for (size_t i = frameIndex; i < frameIndex + requiredFrames; i++) {
                allocationMap[i] = { true, process.getID() };
            }
            process.setMemAllocated(true); // Update process status
            return true;
        }
    } else {
        // Paging memory allocation
        size_t requiredPages = calcFrames(memoryRequired);
        size_t allocatedPages = 0;

        for (size_t i = 0; i < numFrames; i++) {
            if (!allocationMap[i].first) {
                allocationMap[i] = { true, process.getID() };
                allocatedPages++;
                if (allocatedPages == requiredPages) {
                    process.setMemAllocated(true); // Update process status
                    incrementPagedIn();
                    return true;
                }
            }
        }
        for (size_t i = 0; i < numFrames; i++) {
            if (allocationMap[i].second == process.getID()) {
                allocationMap[i].first = false;
                incrementPagedOut();
            }
        }
    }
    return false;
}

void MemoryManager::deallocate(int pid, size_t size) {
    if (isFlatAlloc) {
        for (size_t i = 0; i < numFrames; i++) {
            if (allocationMap[i].second == pid) {
                allocationMap[i].first = false;
            }
        }
    }
    else {
        for (size_t i = 0; i < numFrames; i++) {
            if (allocationMap[i].second == pid) {
                allocationMap[i].first = false;
                incrementPagedOut();
            }
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