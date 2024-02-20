#include "Vec2fAnimation.hpp"

Vec2fAnimation::Vec2fAnimation(sf::Transformable* object, sf::Vector2f to, Animation::Duration duration, float (*blend)(float))
    : Animation(duration, blend)
    , object(object)
    , from({})
    , to(to)
{

}

Vec2fAnimation::Vec2fAnimation()
    : Animation()
    , object(nullptr)
    , from({})
    , to({})
{
}

Vec2fAnimation::~Vec2fAnimation()
{
}

void Vec2fAnimation::moving(float delta)
{

    object->setPosition(from + (to - from) * delta);
}

void Vec2fAnimation::started()
{
    from = object->getPosition();
}

void Vec2fAnimation::finished()
{
    object->setPosition(to);
}
