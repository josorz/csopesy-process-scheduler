#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Process.h"

class MemoryManager
{
public:
	static void initialize(size_t memSize, size_t memPerFrame, size_t memPerProc);
	static MemoryManager* getInstance();

	bool allocate(int pid, size_t size);
	void deallocate(int pid, size_t size);
	bool canAllocate(size_t size, size_t* frameIndex);

	size_t getnNumFrames();
	size_t getMemorySize();
	size_t getMemPerFrame();
	size_t getMemPerProc();
	const std::unordered_map<size_t, std::pair<bool, int>>& getAllocationMap() const;

private: 
	// Singleton Members
	MemoryManager(size_t memSize, size_t memPerFrame, size_t memPerProc);
	static MemoryManager* sharedInstance;

	// Memory
	std::vector<char> memory;
	std::unordered_map<size_t, std::pair<bool, int>> allocationMap;
	size_t maxOverallMem = 0;
	size_t memPerFrame = 0;
	size_t memPerProc = 0;
	size_t numFrames = 0;

	size_t sizeToFrame(size_t size);
};

