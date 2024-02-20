#include "Clock.hpp"


Clock::Time Clock::now()
{
    return std::chrono::high_resolution_clock::now();
}


Clock::Clock()
    : point(Clock::now())
{

}

Clock::Time Clock::startedAt() const
{
    return this->point;
}