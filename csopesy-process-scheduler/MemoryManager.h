//refs:
// https://www.geeksforgeeks.org/first-fit-algorithm-in-memory-management-using-linked-list/
// https://www.geeksforgeeks.org/program-first-fit-algorithm-memory-management/?ref=rp

#pragma once
#include <list>
#include <string>

// Free memory block structure
struct FreeBlock {
    int start;
    int size;
    FreeBlock(int s, int sz) : start(s), size(sz) {}
};

// Allocated memory block structure
struct AllocBlock {
    int start;
    int size;
    std::string processName;
    AllocBlock(int s, int sz, const std::string& name) : start(s), size(sz), processName(name) {}
};

// First-fit memory manager class
class FirstFitMemoryManager {
private:
    int totalMemory;
    int memPerProc;
    std::list<FreeBlock> freeList;
    std::list<AllocBlock> allocList;

public:
    FirstFitMemoryManager(int totalMem, int memPerProc);
    bool allocateMemory(const std::string& processName);
    void freeMemory(const std::string& processName);
    void mergeFreeBlocks();
    int calculateExternalFragmentation() const;

    // Add declarations for memory snapshot generation and timestamp retrieval
    void generateMemorySnapshot(int quantumCycle, const std::string& timestamp);
};
