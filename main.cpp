#include <iostream>

#include "scheduler.hpp"
#include "task.hpp"

Task<std::int32_t> task_c()
{
    std::cout << "task_c start\n";
    co_return 1;
}

Task<std::int32_t> task_b()
{
    std::cout << "task_b start\n";
    co_await task_c();
    std::cout << "task_b end\n";
    co_return 1;
}

Task<std::int32_t> task_a()
{
    std::cout << "task_a start\n";
    co_await task_b();
    std::cout << "task_a end\n";
    co_return 0;
}

int main()
{
    auto      t = task_a();
    Scheduler sheduler;
    sheduler.enqueue(t.handle);
}
