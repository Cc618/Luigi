#pragma once

#include <string>
#include <SFML/Graphics.hpp>

// Whether a key is down
bool pressed(const std::string& name);

// Whether a key is down and was up the last frame
bool typed(const std::string& name);

void set_key_down(const sf::Keyboard::Key& key);

void update_inputs();
