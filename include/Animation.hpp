#pragma once

#include "Clock.hpp"

class Animation
{
 public:
    using Duration = Clock::Duration;

 public:
    enum class State : int
    {
        none = 0,

        pause,
        paused,

        start,
        started,

        moving,

        finish,
        finished,
    };

 public:
    Animation(Duration duration, float (*blend)(float));
    Animation();
    virtual ~Animation() = default;

    void update();
    void updateMoving();


    virtual void moving(float delta) = 0;

    virtual void started();
    virtual void finished();
    // virtual void paused();



 protected:
    State state;
    Clock clock;
    Duration duration;

    float (*blend)(float);
};
