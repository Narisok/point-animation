#pragma once

#include "Clock.hpp"

template<class T>
inline T Clock::elapsed() const
{
    return std::chrono::duration<T>(now() - point).count();
}

template<>
inline Clock::Duration Clock::elapsed<Clock::Duration>() const
{
    return now() - point;
}

template<class T>
inline T Clock::diff(Clock::Time time) const
{
    return std::chrono::duration<T>(time - point).count();
}


template<class T>
inline T Clock::restart()
{
    auto elapsed =  std::chrono::duration<T>(now() - point).count();
    this->point = now();
    return elapsed;
}
