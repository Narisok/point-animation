#pragma once

#include "Animation.hpp"

#include <SFML/Graphics.hpp>

class Vec2fAnimation: public Animation
{
 public:
    Vec2fAnimation(sf::Transformable* object, sf::Vector2f to, Animation::Duration duration = std::chrono::seconds(1), float (*blend)(float) = nullptr);
    Vec2fAnimation();
    ~Vec2fAnimation() override;

    void moving(float delta) override;


    void started() override;
    void finished() override;

 protected:
    sf::Transformable* object;
    sf::Vector2f from;
    sf::Vector2f to;

};
