#pragma once

#include <queue>
#include <mutex>
#include <functional>

class CommandQueue {
public:
    void push(std::function<void()> command);
    bool pop(std::function<void()>& command);

private:
    std::queue<std::function<void()>> queue;
    std::mutex mtx;
};
