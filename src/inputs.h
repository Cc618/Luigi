#pragma once

#include <string>
#include <SFML/Graphics.hpp>

// Whether a key is down
bool pressed(const std::string& key);

// Whether a key is down and was up the last frame
bool typed(const std::string& key);

void set_key_down(const sf::Keyboard::Key& key);

bool mouse_pressed(const std::string& btn);

bool mouse_typed(const std::string& btn);

void update_inputs();

void set_btn_down(const sf::Mouse::Button& btn);

// // Static mouse class
// class Mouse
// {

//     // TODO : x, y

// public:

// };
