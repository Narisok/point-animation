#include "IntAnimation.hpp"

IntAnimation::IntAnimation(Animation::Duration duration, int from, int to)
    : Animation(duration, nullptr)
    , from(from)
    , to(to)
    , current(from)
{

}

IntAnimation::IntAnimation()
    : Animation()
    , from(0)
    , to(0)
    , current(0)
{
}

IntAnimation::~IntAnimation()
{
}

void IntAnimation::moving(float delta)
{
    current = from + (to - from) * delta;
}

void IntAnimation::started()
{
    current = from;
}

void IntAnimation::finished()
{
    current = to;
}
