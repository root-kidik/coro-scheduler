#include <iostream>

#include "task.hpp"

Task<std::int32_t> task_a()
{
    std::cout << "task_a start: " << std::this_thread::get_id() << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "task_a end: " << std::this_thread::get_id() << "\n";
    co_return 1;
}

Task<std::int32_t> task_b()
{
    std::cout << "task_b start: " << std::this_thread::get_id() << "\n";

    std::vector<Task<std::int32_t>> tasks;
    for (std::size_t i = 0; i < std::thread::hardware_concurrency() - 1; i++)
        tasks.emplace_back(task_a()); // 🚀

    for (auto& task : tasks)
        std::cout << co_await task << "\n"; // 🕗

    std::cout << "task_b end: " << std::this_thread::get_id() << "\n";

    co_return 0;
}

int main()
{
    std::cout << "main: " << std::this_thread::get_id() << "\n";
    task_b(); // 🚀

    std::getchar();
}
