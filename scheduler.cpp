#include "scheduler.hpp"

Scheduler::Scheduler(std::size_t num_threads)
{
    _workers.reserve(num_threads);

    for (size_t t = 0; t < num_threads; ++t)
        _workers.emplace_back(
            [this]()
            {
                while (true)
                {
                    std::coroutine_handle<> task;
                    {
                        std::unique_lock<std::mutex> lock(_mtx);
                        _cv.wait(lock, [this] { return _stop || (!_tasks.empty()); });
                        if (_stop)
                            return;

                        task = _tasks.front();
                        _tasks.pop();
                    }

                    task.resume();
                }
            });
}

Scheduler::~Scheduler()
{
    _stop = true;
    _cv.notify_all();

    for (auto& worker : _workers)
        worker.join();
}

void Scheduler::enqueue(std::coroutine_handle<> task)
{
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _tasks.push(task);
    }
    _cv.notify_one();
}
