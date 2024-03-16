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


sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");

sf::View view(sf::FloatRect(-125,-250, 500, 500));

int main()
{

    std::vector<sf::RectangleShape> rects(2, sf::RectangleShape({2.f, 2.f}));

    rects.at(0).setSize({500.f, 2.f});

    rects.at(1).setSize({2.f, 500.f});

    for (auto& r : rects) {
        r.setFillColor(sf::Color::Magenta);
        auto p = r.getSize();
        r.setOrigin({p.x/2.f, p.y/2.f});
        r.setPosition({0.f, 0.f});
    }

    rects.at(0).setPosition({125.f, 0.f});

    sf::RectangleShape rect({200.f, 200.f});
    rect.setOrigin({0, 100});
    rect.setPosition({0.f, 0.f});



    shape.setOrigin({shape.getRadius(), shape.getRadius()});
    shape.setPosition(150, 0.f);
    shape.setFillColor({136, 176, 75});

    window.setView(view);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        loop();


        window.clear();

        window.draw(rect);


        {
            window.draw(shape);

            auto color = shape.getFillColor();
            shape.setFillColor(sf::Color(color.r, color.g, color.b, 100));
            shape.move({0.f, -25.f});
            window.draw(shape);
            shape.move({0.f, 25.f});
            shape.setFillColor(color);

            sf::Vector2f p = shape.getPosition();
            sf::Vector2i pi(static_cast<int>(p.x), static_cast<int>(p.y));

            int max = 200;
            float radius = shape.getRadius();
            int ri = static_cast<int>(radius);

            if ((pi.x + ri) > max ) {
                shape.setPosition({float(((pi.x + ri) % max) - ri), p.y});

                window.draw(shape);
                shape.setPosition(p);
            }


            for (auto& r : rects) {
                window.draw(r);
            }
        }

        window.display();


        std::this_thread::sleep_for(15ms);
    }

    return 0;
}


void loop()
{
    sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2f p = shape.getPosition();


    shape.setPosition({mp.x, p.y});
}
