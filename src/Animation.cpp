#include "Animation.hpp"

#include <iostream>

Animation::Animation(Animation::Duration duration, float (*blend)(float))
    : state(Animation::State::start)
    , clock()
    , duration(duration)
    , blend(blend)
{

}

Animation::Animation()
    : state(Animation::State::none)
    , clock()
    , duration(Animation::Duration(0))
    , blend(nullptr)
{
}

void Animation::update()
{
    switch (this->state)
    {
    case State::none:

        break;

    case State::start:
        this->clock.restart();

        this->state = State::started;
        // break;

    case State::started:
        this->started();

        this->state = State::moving;
        break;

    case State::moving:
        this->updateMoving();
        break;


    case State::finish:
        // this->paused();

        this->state = State::finished;
        // break;

    case State::finished:
        this->finished();
        break;
    }
}

void Animation::updateMoving()
{
    auto elapsed = this->clock.elapsed<Clock::Duration>();

    if (elapsed >= this->duration) {
        this->state = State::finish;
        return update();
    }


    float delta = std::chrono::duration<float>(elapsed) / std::chrono::duration<float>(this->duration);

    if (this->blend) {
        delta = this->blend(delta);
    }

    this->moving(delta);
}

void Animation::started()
{
}

void Animation::finished()
{
}
