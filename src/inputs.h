#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace lg
{
    // Static mouse class
    class Mouse
    {
    public:
        static std::pair<int, int> get_pos();
        static void set_pos(const std::pair<int, int>& xy);
    };
}
