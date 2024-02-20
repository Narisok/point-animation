#pragma once

#include "Animation.hpp"

class IntAnimation: public Animation
{
 public:
    IntAnimation(Animation::Duration duration, int from, int to);
    IntAnimation();
    ~IntAnimation() override;

    void moving(float delta) override;


    void started() override;
    void finished() override;

    inline int get() const { return current; }

 protected:
    int from;
    int to;

    int current;
};
