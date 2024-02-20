#pragma once

#include <chrono>

class Clock
{
 public:
    using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using Duration = std::chrono::nanoseconds;

 public:

    Clock();
    ~Clock() = default;

    static Time now();

    Time startedAt() const;

    template<class T = float>
    inline T restart();

    template<class T = float>
    inline T elapsed() const;


    template<class T = float>
    inline T diff(Time) const;


 protected:
    Time point;
};

#include "Clock.inc.hpp"
