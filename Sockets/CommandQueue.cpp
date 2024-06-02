#include "CommandQueue.hpp"

void CommandQueue::push(std::function<void()> command) {
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(command);
}

bool CommandQueue::pop(std::function<void()>& command) {
    std::lock_guard<std::mutex> lock(mtx);
    if (queue.empty()) {
        return false;
    }
    command = std::move(queue.front());
    queue.pop();
    return true;
}
