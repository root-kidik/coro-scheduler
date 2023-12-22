#include "scheduler.hpp"

template <typename T>
constexpr std::suspend_always Task<T>::promise_type::initial_suspend() const noexcept
{
    return {};
}

template <typename T>
constexpr std::suspend_always Task<T>::promise_type::final_suspend() noexcept
{
    is_ready.test_and_set();
    is_ready.notify_all();
    return {};
}

template <typename T>
constexpr Task<T> Task<T>::promise_type::get_return_object()
{
    return std::coroutine_handle<promise_type>::from_promise(*this);
}

template <typename T>
constexpr void Task<T>::promise_type::return_value(T&& value)
{
    data = std::forward<T>(value);
}

template <typename T>
constexpr void Task<T>::promise_type::unhandled_exception() const noexcept
{
}

template <typename T>
constexpr bool Task<T>::await_ready() const noexcept
{
    return handle.done();
}

template <typename T>
constexpr std::coroutine_handle<typename Task<T>::promise_type> Task<T>::await_suspend(
    std::coroutine_handle<typename Task<T>::promise_type> awaiting_coroutine) const noexcept
{
    handle.promise().is_ready.wait(false);

    return awaiting_coroutine;
}

template <typename T>
constexpr T Task<T>::await_resume() const noexcept
{
    return handle.promise().data;
}

template <typename T>
constexpr Task<T>::Task(std::coroutine_handle<promise_type> handle) : handle(handle)
{
    sch.enqueue(handle);
}

constexpr std::suspend_always Task<void>::promise_type::initial_suspend() const noexcept
{
    return {};
}

constexpr std::suspend_always Task<void>::promise_type::final_suspend() noexcept
{
    is_ready.test_and_set();
    is_ready.notify_all();
    return {};
}

constexpr Task<void> Task<void>::promise_type::get_return_object()
{
    return std::coroutine_handle<promise_type>::from_promise(*this);
}

constexpr void Task<void>::promise_type::return_void() const noexcept
{
}

constexpr void Task<void>::promise_type::unhandled_exception() const noexcept
{
}

constexpr bool Task<void>::await_ready() const noexcept
{
    return handle.done();
}

constexpr std::coroutine_handle<typename Task<void>::promise_type> Task<void>::await_suspend(
    std::coroutine_handle<typename Task<void>::promise_type> awaiting_coroutine) const noexcept
{
    handle.promise().is_ready.wait(false);

    return awaiting_coroutine;
}

constexpr void Task<void>::await_resume() const noexcept
{
}

constexpr Task<void>::Task(std::coroutine_handle<promise_type> handle) : handle(handle)
{
    sch.enqueue(handle);
}
