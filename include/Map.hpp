#pragma once

#include <SFML/Graphics.hpp>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <iomanip>

// #define USE_TEXT_VALUE
#define SHAPE_SIZE  10.f
#define SHAPE_OUTLINE_SIZE  0.f

struct BaseFont
{

    inline static const sf::Font& getFont()
    {
        if (! loaded) {
            loaded = font.loadFromFile("fonts/Roboto-Regular.ttf");

            if (loaded) {
                font.setSmooth(false);
            }
        }

        return font;
    }

 private:
    inline static sf::Font font = {};
    inline static bool loaded = false;
};

struct Point: public sf::Drawable
{
    float value;
    float delta;
    // float delta;

    sf::RectangleShape shape;
    sf::Text text;

    Point *neighbors[4];
    unsigned int neighborsCount;

    Point()
        : value(0.f)
        , delta(0.f)
        , shape({SHAPE_SIZE, SHAPE_SIZE})
        , text()
        , neighbors{nullptr, nullptr, nullptr, nullptr}
        , neighborsCount(0)
    {
        text.setCharacterSize(11);
        // text.setStyle(sf::Text::Style::Bold);
        text.setFont(BaseFont::getFont());

        updateText();
    }

    inline void resetStyle()
    {
        int m = static_cast<int>(std::min(value, 255.f));

        shape.setFillColor(sf::Color(m + 20 > 255 ? 255 : m + 20, value > 1.f ? 20 : 0, value > 1.f ? 20 : 0));

        text.setFillColor({220, 220, 220});
    }

    inline void hover()
    {
        shape.setFillColor({200, 200, 200});
        text.setFillColor({80, 80, 80});
    }

    inline void setNeighbors(
        Point *top = nullptr,
        Point *bottom = nullptr,
        Point *left = nullptr,
        Point *right = nullptr
    )
    {
        neighbors[0] = top;
        neighbors[1] = bottom;
        neighbors[2] = left;
        neighbors[3] = right;

        neighborsCount = (top ? 1 : 0) + (bottom ? 1 : 0) + (left ? 1 : 0) + (right ? 1 : 0);
    }

    void update()
    {
        // text.set
        updateNeighbors();

        // updateText();
        updateShape();
    }

    void setValue(float value)
    {
        this->value = value;

        updateText();
    }

    void addValue(float value)
    {
        this->value += value;

        updateText();
    }

    inline Point *top()
    {
        return neighbors[0];
    }

    inline Point *bottom()
    {
        return neighbors[1];
    }

    inline Point *left()
    {
        return neighbors[2];
    }

    inline Point *right()
    {
        return neighbors[3];
    }

    void updateNeighbors()
    {
        if (value < 0.01f) {
            return;
        }

        float maxGive = value * 0.8;

        float maxGiveToNeighbor = maxGive / 4;

        if (maxGiveToNeighbor < 0.01f) {
            return;
        }


        for (auto neighbor : neighbors) {
            if (neighbor) {

                neighbor->delta += maxGiveToNeighbor;
                this->delta -= maxGiveToNeighbor;
                    // neighbor->addValue(maxGiveToNeighbor);
                    // value -= maxGiveToNeighbor;


            }
        }

        updateText();
    }

    void updateValue()
    {
        this->value += this->delta;
        this->delta = 0.f;

        updateText();
    }

    void updateShape()
    {

    }

    void updateText()
    {
        #ifdef USE_TEXT_VALUE
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << value;

        text.setString(stream.str());
        #endif
    }

    void setPosition(sf::Vector2f position)
    {
        shape.setPosition(position);
        text.setPosition(position);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(shape, states);
        #ifdef USE_TEXT_VALUE
        target.draw(text, states);
        #endif
    }
};

template <class T>
struct MatrixRow;

template <class T>
struct Matrix
{
    size_t rows;
    size_t columns;

    std::vector<T> elements;

    Matrix(size_t rows, size_t columns)
        : rows(rows)
        , columns(columns)
        , elements(rows * columns)
    {
        for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < columns; col++) {
                elements[columns * row + col].setPosition({
                    col * (SHAPE_SIZE + SHAPE_OUTLINE_SIZE),
                    row * (SHAPE_SIZE + SHAPE_OUTLINE_SIZE)
                });
            }
        }
    }

    MatrixRow<T> operator[](size_t row);
};


template <class T>
struct MatrixRow
{
    size_t row;
    Matrix<T> *matrix;

    inline T& operator[](size_t column)
    {
        return matrix->elements.at(row * matrix->columns + column);
    }
};

class Map: public sf::Drawable, public sf::Transformable
{
 public:
    using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using Duration = std::chrono::nanoseconds;

    using BaseDuration = std::chrono::milliseconds;

//  protected:
 public:

    Matrix<Point> matrix;

    Time time;
    Duration stepDuration;
    bool updating;
    bool drawing;

 public:

    inline Map(size_t rows = 3, size_t columns = 3, Duration step = BaseDuration(500))
        : Drawable()
        , Transformable()
        , matrix(rows, columns)
        , time(std::chrono::high_resolution_clock::now())
        , stepDuration(step)
        , updating(false)
        , drawing(false)
    {
        for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < columns; col++) {
                matrix[row][col].setNeighbors(
                    row > 0 ? &matrix[row-1][col] : nullptr,
                    row + 1 < rows ? &matrix[row+1][col] : nullptr,
                    col > 0 ? &matrix[row][col-1] : nullptr,
                    col + 1 < columns ? &matrix[row][col+1] : nullptr
                );
            }
        }

    }

    virtual ~Map()
    {

    }


    void update()
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<Duration>(now - time);

        for (; elapsed >= stepDuration; elapsed -= stepDuration) {

            if (updating) {
                updateStep();
            }
        }

        time = now - elapsed;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        if (drawing) {
            for (auto &i : matrix.elements) {
                target.draw(i, states);
            }
        }
    }

    inline MatrixRow<Point> operator[](size_t row)
    {
        return matrix[row];
    }

 protected:

    void updateStep()
    {
        for (auto &point : matrix.elements) {
            point.update();
        }

        for (auto &point : matrix.elements) {
            point.updateValue();
        }
    }
};

template <class T>
inline MatrixRow<T> Matrix<T>::operator[](size_t row)
{
    return MatrixRow<T>{
        .row=row,
        .matrix=this
    };
}
