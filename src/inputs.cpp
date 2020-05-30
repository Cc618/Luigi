#include "inputs.h"
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "error.h"

using namespace std;
using namespace sf;

// Use lower case ids separated by spaces
unordered_map<std::string, Keyboard::Key> id_to_kbd = {
    // Letters
    { "a", Keyboard::A },
    { "b", Keyboard::B },
    { "c", Keyboard::C },
    { "d", Keyboard::D },
    { "e", Keyboard::E },
    { "f", Keyboard::F },
    { "g", Keyboard::G },
    { "h", Keyboard::H },
    { "i", Keyboard::I },
    { "j", Keyboard::J },
    { "k", Keyboard::K },
    { "l", Keyboard::L },
    { "m", Keyboard::M },
    { "n", Keyboard::N },
    { "o", Keyboard::O },
    { "p", Keyboard::P },
    { "q", Keyboard::Q },
    { "r", Keyboard::R },
    { "s", Keyboard::S },
    { "t", Keyboard::T },
    { "u", Keyboard::U },
    { "v", Keyboard::V },
    { "w", Keyboard::W },
    { "x", Keyboard::X },
    { "y", Keyboard::Y },
    { "z", Keyboard::Z },

    // Digits
    { "0", Keyboard::Num0 },
    { "1", Keyboard::Num1 },
    { "2", Keyboard::Num2 },
    { "3", Keyboard::Num3 },
    { "4", Keyboard::Num4 },
    { "5", Keyboard::Num5 },
    { "6", Keyboard::Num6 },
    { "7", Keyboard::Num7 },
    { "8", Keyboard::Num8 },
    { "9", Keyboard::Num9 },

    // Arrows
    { "left", Keyboard::Left },
    { "up", Keyboard::Up },
    { "right", Keyboard::Right },
    { "down", Keyboard::Down },

    // Functions
    { "f1", Keyboard::F1 },
    { "f2", Keyboard::F2 },
    { "f3", Keyboard::F3 },
    { "f4", Keyboard::F4 },
    { "f5", Keyboard::F5 },
    { "f6", Keyboard::F6 },
    { "f7", Keyboard::F7 },
    { "f8", Keyboard::F8 },
    { "f9", Keyboard::F9 },
    { "f10", Keyboard::F10 },
    { "f11", Keyboard::F11 },
    { "f12", Keyboard::F12 },
    { "f13", Keyboard::F13 },
    { "f14", Keyboard::F14 },
    { "f15", Keyboard::F15 },

    // Specials
    { "space", Keyboard::Space },
    { "escape", Keyboard::Escape },
    { "enter", Keyboard::Enter },
    { "delete", Keyboard::Delete },
    { "shift", Keyboard::LShift },
    { "alt", Keyboard::LAlt },
    { "control", Keyboard::LControl },
    { "tab", Keyboard::Tab },
};

// To lower
static inline string name_to_id(const string& name)
{
    string result = name;

    transform(result.begin(), result.end(), result.begin(), ::tolower);

    return result;
}

bool pressed(const std::string& name)
{
    // Find key
    string id = name_to_id(name);
    auto key = id_to_kbd.find(id);

    Error::check(key != id_to_kbd.end(), "Unknown key with name '" + name + "'");

    // Test key
    return Keyboard::isKeyPressed((*key).second);
}
