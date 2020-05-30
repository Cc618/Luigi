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
    { "left", Keyboard::Left },
    { "up", Keyboard::Up },
    { "right", Keyboard::Right },
    { "down", Keyboard::Down },
};

// To lower
static string name_to_id(const string& name)
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
