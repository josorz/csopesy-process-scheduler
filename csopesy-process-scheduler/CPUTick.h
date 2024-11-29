#include <atomic>

class CPUTick {
private:
    std::atomic<int> tick;
    std::atomic<int> activeTicks;
    std::atomic<int> idleTicks;
    CPUTick() : tick(0), activeTicks(0), idleTicks(0) {}

public:
    static CPUTick& getInstance() {
        static CPUTick instance;
        return instance;
    }

    void addTick() {
        tick.fetch_add(1, std::memory_order_relaxed);
    }

    void addActiveTick() {
        activeTicks.fetch_add(1, std::memory_order_relaxed);
    }

    void addIdleTick() {
        idleTicks.fetch_add(1, std::memory_order_relaxed);
    }

    int getTick() const {
        return tick.load(std::memory_order_relaxed);
    }

    int getActiveTicks() const {
        return activeTicks.load(std::memory_order_relaxed);
    }
    
    int getIdleTicks() const {
        return idleTicks.load(std::memory_order_relaxed);
    }
};
