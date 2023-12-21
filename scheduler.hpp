#pragma once

#include <atomic>
#include <condition_variable>
#include <coroutine>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class Scheduler
{
public:
    Scheduler(std::size_t num_threads = std::thread::hardware_concurrency());
    ~Scheduler();

    void enqueue(std::coroutine_handle<> task);

private:
    std::queue<std::coroutine_handle<>> _tasks;
    std::vector<std::thread>            _workers;

    std::mutex              _mtx;
    std::condition_variable _cv;
    std::atomic<bool>       _stop{false};
};

inline Scheduler sch;
