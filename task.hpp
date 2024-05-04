#pragma once

#include <coroutine>
#include <iostream>

template <typename T>
struct Task
{
    struct promise_type
    {
        constexpr std::suspend_always initial_suspend() const noexcept
        {
            return {};
        };
        constexpr auto final_suspend() noexcept
        {
            struct final_awaiter
            {
                bool await_ready() noexcept
                {
                    return false;
                }

                void await_suspend(std::coroutine_handle<promise_type> callee) noexcept
                {
                    if (auto continuation = callee.promise().continuation; continuation)
                        continuation.resume();
                }

                void await_resume() noexcept
                {
                }
            };

            return final_awaiter{};
        };

        constexpr Task get_return_object()
        {
            return std::coroutine_handle<promise_type>::from_promise(*this);
        }
        constexpr void return_value(T&& value)
        {
            data = std::forward<T>(value);
        }
        constexpr void unhandled_exception() const noexcept
        {
        }

        T                       data;
        std::coroutine_handle<> continuation;
    };

    constexpr Task(std::coroutine_handle<promise_type> handle) : handle{handle}
    {
    }


    ~Task()
    {
        handle.destroy();
    }

    constexpr bool await_ready() const noexcept
    {
        return handle.done();
    }

    constexpr std::coroutine_handle<promise_type> await_suspend(std::coroutine_handle<promise_type> caller) const noexcept
    {
        handle.promise().continuation = caller;
        return handle;
    }

    constexpr T await_resume() const noexcept
    {
        return handle.promise().data;
    }

    std::coroutine_handle<promise_type> handle;
};
