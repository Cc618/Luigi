#include "inputs.h"
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "error.h"
#include "game.h"

using namespace std;
using namespace sf;
using namespace lg;

// Use lower case ids separated by spaces
unordered_map<std::string, Keyboard::Key> id_to_key = {
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

unordered_map<sf::Keyboard::Key, bool> typed_keys;

std::unordered_map<std::string, sf::Mouse::Button> id_to_btn = {
    { "left", sf::Mouse::Button::Left },
    { "right", sf::Mouse::Button::Right },
    { "middle", sf::Mouse::Button::Middle },
};

std::unordered_map<sf::Mouse::Button, bool> typed_btns;

// To lower
static inline string name_to_id(const string& name)
{
    string result = name;

    transform(result.begin(), result.end(), result.begin(), ::tolower);

    return result;
}

// --- Inputs --- //
bool Game::pressed(const std::string& name) const
{
    // Find key
    string id = name_to_id(name);
    auto key = id_to_key.find(id);

    Error::check(key != id_to_key.end(), "Unknown key with name '" + name + "'");

    // Test key
    return Keyboard::isKeyPressed((*key).second);
}

bool Game::typed(const std::string& name) const
{
    // Find key
    string id = name_to_id(name);
    auto key = id_to_key.find(id);

    Error::check(key != id_to_key.end(), "Unknown key with name '" + name + "'");

    // Test key
    return typed_keys[(*key).second];
}

void Game::set_key_down(const sf::Keyboard::Key& key) const
{
    typed_keys[key] = true;
}

bool Game::mouse_pressed(const std::string& name) const
{
    // Find key
    string id = name_to_id(name);
    auto btn = id_to_btn.find(id);

    Error::check(btn != id_to_btn.end(), "Unknown mouse button with name '" + name + "'");

    return sf::Mouse::isButtonPressed((*btn).second);
}

bool Game::mouse_typed(const std::string& name) const
{
    // Find key
    string id = name_to_id(name);
    auto btn = id_to_btn.find(id);

    Error::check(btn != id_to_btn.end(), "Unknown mouse button with name '" + name + "'");

    return typed_btns[(*btn).second];
}

void Game::update_inputs() const
{
    // Set all keys to false
    for (auto p : id_to_key)
        typed_keys[p.second] = false;
    
    // Set all buttons to false
    for (auto p : id_to_btn)
        typed_btns[p.second] = false;
}

void Game::set_btn_down(const sf::Mouse::Button& btn) const
{
    typed_btns[btn] = true;
}

// --- Mouse --- //
std::pair<int, int> lg::Mouse::get_pos()
{
    Error::check(Game::instance != nullptr && Game::instance->win != nullptr,
        "A game must be running to use this function");

    auto pos = sf::Mouse::getPosition(*Game::instance->win);
    auto size = Game::instance->win->getSize();
    return std::make_pair(pos.x, size.y - pos.y);
}

void lg::Mouse::set_pos(const std::pair<int, int>& xy)
{
    auto size = Game::instance->win->getSize();
    sf::Mouse::setPosition(Vector2i(xy.first, size.y - xy.second), *Game::instance->win);
}

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace std;

void bind_inputs(py::module &m)
{
    // Mouse
    py::class_<lg::Mouse>(m, "Mouse")
        .def_property_static("pos",
            [](py::object) -> std::pair<int, int> { return lg::Mouse::get_pos(); },
            [](py::object, const std::pair<int, int>& xy) { lg::Mouse::set_pos(xy); },
            "The position is from the bottom left corner of the window."
        )

        .doc() = R"(
            (**inputs**) Handles the mouse, this class is static.

            :property pos: Describes the mouse position in a int 2-tuple.
        )"
    ;

}
