#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include "Process.h"

class MemoryManager {
public:
    // Initialize the singleton instance
    static void initialize(size_t memSize, size_t memPerFrame, bool isFlatAlloc);

    // Get the singleton instance
    static MemoryManager* getInstance();

    // Allocate memory for a process
    bool allocateMem(Process& process);

    // Deallocate memory for a process
    void deallocate(int pid, size_t size);

    // Get allocation map (for flat memory)
    const std::vector<std::pair<bool, int>>& getAllocationMap() const;

    // Get memory size
    size_t getMemorySize() const { return maxOverallMem; };

    // Get memory size per frame
    size_t getMemPerFrame() const { return memPerFrame; };

    // Get total number of frames
    size_t getNumFrames() const { return numFrames; };

    size_t getFreeFrames() { return freeFrameList.size(); }
  
    // Get total used memory
    size_t getUsedMemory() const;

    void incrementPagedIn() { numPagedIn++; };
    void incrementPagedOut() { numPagedOut++; };
    size_t getPagedInCount() const { return numPagedIn; };
    size_t getPagedOutCount() const { return numPagedOut; };
    
private:
    // Singleton instance
    static MemoryManager* sharedInstance;

    // Private constructor
    MemoryManager(size_t memSize, size_t memPerFrame, bool isFlatAlloc);

    // Convert size to frames/pages
    size_t calcFrames(size_t size) const;

    // Check if memory can be allocated in flat mode
    bool canAllocateFlat(size_t size, size_t* frameIndex);

    // Member variables
    size_t maxOverallMem; // Total memory size
    size_t memPerFrame;   // Size of each frame/page
    size_t numFrames;     // Total number of frames/pages
    bool isFlatAlloc;     // Flat allocation mode flag
    size_t numPagedIn;
    size_t numPagedOut;

    // Memory structures
    std::vector<char> memory;                              // Physical memory representation
    std::vector<std::pair<bool, int>> allocationMap;       // Allocation map
    std::vector <size_t> freeFrameList;
};
