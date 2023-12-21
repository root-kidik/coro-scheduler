#include "scheduler.hpp"

constexpr std::suspend_always Task::promise_type::initial_suspend() const noexcept
{
    return {};
}

constexpr std::suspend_always Task::promise_type::final_suspend() noexcept
{
    is_ready.test_and_set();
    is_ready.notify_all();
    return {};
}

constexpr Task Task::promise_type::get_return_object()
{
    return std::coroutine_handle<promise_type>::from_promise(*this);
}

constexpr void Task::promise_type::return_void() const noexcept
{
}

constexpr void Task::promise_type::unhandled_exception() const noexcept
{
}

constexpr bool Task::awaiter::await_ready() const noexcept
{
    return coro.done();
}

constexpr std::coroutine_handle<Task::promise_type> Task::awaiter::await_suspend(
    std::coroutine_handle<Task::promise_type> awaiting_coroutine) noexcept
{
    coro.promise().is_ready.wait(false);

    return awaiting_coroutine;
}

constexpr void Task::awaiter::await_resume() const noexcept
{
}

constexpr Task::Task(std::coroutine_handle<promise_type> handle) : handle(handle)
{
    sch.enqueue(handle);
}

constexpr std::coroutine_handle<Task::promise_type> Task::get_handle() const noexcept
{
    return handle;
}

constexpr Task::awaiter Task::operator co_await() const noexcept
{
    return {handle};
}
