#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace lg
{
    // Static mouse class
    class Mouse
    {
    public:
        // The position is from the bottom left corner of the window
        static std::pair<int, int> get_pos();
        static void set_pos(const std::pair<int, int>& xy);
    };
}
