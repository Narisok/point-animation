#include <chrono>
#include <iostream>
using namespace std::chrono_literals;

int main()
{
    constexpr auto duration = 123ms;
    std::cout << duration << '\n';
}