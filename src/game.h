#pragma once

// Handles the window and the game environment

#include <string>

class Game
{
public:
    Game() = default;
    ~Game() = default;

public:
    void start(const std::string& title, int width, int height, float fps=60);
};
