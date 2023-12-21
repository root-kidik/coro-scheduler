#pragma once

#include <atomic>
#include <coroutine>

struct Task
{
    struct promise_type
    {
        constexpr std::suspend_always initial_suspend() const noexcept;
        constexpr std::suspend_always final_suspend() noexcept;

        constexpr Task get_return_object();
        constexpr void return_void() const noexcept;
        constexpr void unhandled_exception() const noexcept;

        std::atomic_flag is_ready;
    };

    struct awaiter
    {
        constexpr bool await_ready() const noexcept;
        constexpr std::coroutine_handle<promise_type> await_suspend(std::coroutine_handle<promise_type> awaiting_coroutine) noexcept;
        constexpr void await_resume() const noexcept;

        std::coroutine_handle<promise_type> coro;
    };

    constexpr Task(std::coroutine_handle<promise_type> handle);

    constexpr std::coroutine_handle<promise_type> get_handle() const noexcept;

    constexpr awaiter operator co_await() const noexcept;

    std::coroutine_handle<promise_type> handle;
};

#include "task.inl"
