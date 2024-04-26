#include <SFML/Graphics.hpp>

#include <chrono>
#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <functional>

#include "Clock.hpp"

#include "AllAnimations.hpp"

#include "Map.hpp"


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


sf::CircleShape shape(20.f);

Clock tickTimer{};
Clock globalTimer{};

float speed = 100;

void loop(sf::RenderWindow&, sf::View&);

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


Map map(90, 90, 10ms);


void foo()
{
    std::this_thread::sleep_for(1ms);
}


void bar()
{

}

int main()
{

    sf::VertexArray vArray;

    size_t cols = 2;
    size_t rows = 3;

    vArray.resize(cols*rows);
    vArray.setPrimitiveType(sf::PrimitiveType::Triangles);

    vArray[0].position = {0.f, 0.f};
    vArray[1].position = {74.f, 21.f};
    vArray[2].position = {27.f, 125.f};


    vArray[3].position = {74.f, 21.f};
    vArray[4].position = {27.f, 125.f};
    vArray[5].position = {74.f, 125.f};
    // for(size_t row = 0; row < rows; row++) {
    //     for(size_t col = 0; col < cols; col++) {
    //             vArray[row * cols + col].position = sf::Vector2f{col * 100.f + (rand()%50), row * 100.f + (rand()%50)};
    //     }
    // }

    sf::RenderWindow window(sf::VideoMode(
        (SHAPE_SIZE+SHAPE_OUTLINE_SIZE) * map.matrix.columns - SHAPE_OUTLINE_SIZE,
        (SHAPE_SIZE+SHAPE_OUTLINE_SIZE) * map.matrix.rows - SHAPE_OUTLINE_SIZE),
        "SFML works!"
    );

    sf::View view(sf::FloatRect(0,0, window.getSize().x, window.getSize().y));

    window.setView(view);

    sf::Text fpsText("0", BaseFont::getFont());
    fpsText.setStyle(sf::Text::Style::Bold);
    fpsText.setCharacterSize(12);

    auto fpsClock = std::chrono::high_resolution_clock::now();

    size_t energy = 0;

    while (window.isOpen())
    {
        for (auto &point : map.matrix.elements) {
            point.resetStyle();
        }

        Point *point = nullptr;

        int row = static_cast<int>(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y) / (int)(SHAPE_SIZE + SHAPE_OUTLINE_SIZE);
        int col = static_cast<int>(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x) / (int)(SHAPE_SIZE + SHAPE_OUTLINE_SIZE);

        if ((row >= 0 && row < static_cast<int>(map.matrix.rows)) && (col >= 0 && col < static_cast<int>(map.matrix.columns))) {
            point = &map[row][col];
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
                std::cout << "A" << std::endl;
                point->update();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Comma) {
                std::cout << "Comma" << std::endl;
                energy -= (int)point->value;
                point->setValue(0.f);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Quote) {
                std::cout << "Quote" << std::endl;
                point->addValue(100.f);
                energy += 100;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B) {
                std::cout << "Bomb" << std::endl;
                point->addValue(10000.f);
                energy += 10000;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M) {
                std::cout << "Mega" << std::endl;
                point->addValue(100000.f);
                energy += 100000;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
                std::cout << "E" << std::endl;
                map.updating = ! map.updating;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O) {
                std::cout << "O" << std::endl;
                for (auto &i : map.matrix.elements) {
                    i.update();
                }
                for (auto &i : map.matrix.elements) {
                    i.updateValue();
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) {
                std::cout << "U" << std::endl;
                map.drawing = ! map.drawing;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                std::cout << "R" << std::endl;
                for (auto &i : map.matrix.elements) {
                    i.value = 0;
                }
                energy = 0;
            }
        }

        if (point) {
            point->hover();
        }




        window.clear();

        loop(window, view);

        map.update();

        window.draw(map);

        window.draw(vArray, sf::RenderStates(sf::Transform().translate({100, 100})));

        auto primitiveBefore = vArray.getPrimitiveType();


        // vArray.setPrimitiveType(sf::PrimitiveType::LineStrip);
        // for(size_t i = 0; i < cols * rows; i++) {
        //     vArray[i].color = sf::Color::Blue;
        // }
        // window.draw(vArray, sf::RenderStates(sf::Transform().translate({100, 100})));

        vArray.setPrimitiveType(sf::PrimitiveType::Points);
        for(size_t i = 0; i < cols * rows; i++) {
            vArray[i].color = sf::Color::Red;
        }
        window.draw(vArray, sf::RenderStates(sf::Transform().translate({100, 100})));


        vArray.setPrimitiveType(primitiveBefore);
        for(size_t i = 0; i < cols * rows; i++) {
            vArray[i].color = sf::Color::White;
        }

        window.draw(fpsText);

        window.display();

        auto elapsed = (std::chrono::high_resolution_clock::now() - fpsClock);
        fpsClock = std::chrono::high_resolution_clock::now();
        if (elapsed.count() > 0) {
            // fpsText.setString("--");
            std::string str = std::to_string(
                std::chrono::duration_cast<std::chrono::nanoseconds>(1s).count()
                 /
                 std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count()
                );

            float aenergy = 0;
            for (auto &i : map.matrix.elements) {
                aenergy += i.value;
            }

            str += std::string("\nTE: ") + std::to_string(energy) + std::string("\nAE: ") + std::to_string(aenergy);

            fpsText.setString(str);

        } else {
            fpsText.setString("Inf");
        }
        std::this_thread::sleep_for(15ms);
    }

    return 0;
}


void loop(sf::RenderWindow &window, sf::View &view)
{
    // map;
}
