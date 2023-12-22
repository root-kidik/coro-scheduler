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
    data = std::move(value);
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
    std::coroutine_handle<typename Task<T>::promise_type> awaiting_coroutine) noexcept
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
