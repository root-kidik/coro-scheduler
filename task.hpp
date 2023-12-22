#pragma once

#include <atomic>
#include <coroutine>

template <typename T>
struct Task
{
    struct promise_type
    {
        constexpr std::suspend_always initial_suspend() const noexcept;
        constexpr std::suspend_always final_suspend() noexcept;

        constexpr Task get_return_object();
        constexpr void return_value(T&& value);
        constexpr void unhandled_exception() const noexcept;

        std::atomic_flag is_ready;
        T                data;
    };

    constexpr Task(std::coroutine_handle<promise_type> handle);

    constexpr bool                                await_ready() const noexcept;
    constexpr std::coroutine_handle<promise_type> await_suspend(
        std::coroutine_handle<promise_type> awaiting_coroutine) const noexcept;
    constexpr T await_resume() const noexcept;

    std::coroutine_handle<promise_type> handle;
};

template <>
struct Task<void>
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

    constexpr Task(std::coroutine_handle<promise_type> handle);

    constexpr bool                                await_ready() const noexcept;
    constexpr std::coroutine_handle<promise_type> await_suspend(
        std::coroutine_handle<promise_type> awaiting_coroutine) const noexcept;
    constexpr void await_resume() const noexcept;

    std::coroutine_handle<promise_type> handle;
};


#include "task.inl"
