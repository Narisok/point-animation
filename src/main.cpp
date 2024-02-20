#include <SFML/Graphics.hpp>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <functional>

#include "Clock.hpp"

#include "AllAnimations.hpp"


using namespace std::chrono_literals;

template <class T>
std::enable_if_t<not std::numeric_limits<T>::is_integer, bool>
float_equal(T x, T y, std::size_t n = 1)
{
    const T m = std::min(std::fabs(x), std::fabs(y));

    const int exp = m < std::numeric_limits<T>::min()
                  ? std::numeric_limits<T>::min_exponent - 1
                  : std::ilogb(m);

    return std::fabs(x - y) <= n * std::ldexp(std::numeric_limits<T>::epsilon(), exp);
}

void loop();

sf::CircleShape shape(20.f);

Clock tickTimer{};
Clock globalTimer{};

float speed = 100;

// inline void moveTo(sf::Transformable &object, sf::Vector2f to, float time = 1.f, std::function<void (sf::Transformable &object)> onComplete)
// {
//     auto cp = object.getPosition();

//     if (float_equal(cp.x, to.x) && float_equal(cp.y, to.y)) {
//         onComplete(object);
//         return;
//     }


// }

float parabolic(float x)
{
    return x*x;
}

float linear(float x)
{
    return x;
}

float bounceUp(float x)
{
    return (x * (x - 1.25f)) * -4.f;
}

float centred(float x)
{
    return std::pow((x - 0.5f),3) * 4.f + 0.5f;
}

Animation* anim;

int main()
{

    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");

    shape.setOrigin(10.f, 10.f);
    shape.setPosition(50.f, 250.f);
    shape.setFillColor({136, 176, 75});

    std::vector<sf::CircleShape> points(10);

    for (auto &point : points) {
        point.setRadius(5.f);
        point.setOrigin({2.5f, 2.5f});

        point.setFillColor({224, 181, 137});
    }

    for (size_t i = 0; i < 5; i++)
    {
        points[1 + i].setPosition(50.f + i * speed, 250.f);
    }


    points[9].setPosition(0.f, 260.f);
    points[9].setFillColor({232, 181, 206});

    bool firstLoop = true;

    tickTimer.restart();
    globalTimer.restart();

    anim = new Vec2fAnimation(&shape, shape.getPosition() + sf::Vector2f({speed, 0.f}), 2s, bounceUp);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (! firstLoop) {
            loop();
        }

        points[9].setPosition(shape.getPosition().x, points[9].getPosition().y);


        window.clear();
        window.draw(shape);


        for (auto &point : points) {
            window.draw(point);
        }
        window.display();

        std::cout << "FPS: " << std::ceil(1.f / tickTimer.restart()) << std::endl;

        std::this_thread::sleep_for(15ms);

        if (firstLoop) {
            std::this_thread::sleep_for(1s);
            firstLoop = false;
        }
    }

    return 0;
}


void loop()
{
    anim->update();
}
