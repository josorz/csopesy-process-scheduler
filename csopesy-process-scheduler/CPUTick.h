#include <atomic>

class CPUTick {
private:
    std::atomic<int> tick;
    CPUTick() : tick(0) {}

public:
    static CPUTick& getInstance() {
        static CPUTick instance;
        return instance;
    }

    void addTick() {
        tick.fetch_add(1, std::memory_order_relaxed);
    }

    int getTick() const {
        return tick.load(std::memory_order_relaxed);
    }
};
