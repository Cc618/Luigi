#pragma once

// Handles the window and the game environment

#include <string>

class Game
{
public:
    Game() = default;
    virtual ~Game() = default;

public:
    // TODO : Use config (for fullscreen...)
    void start(const std::string& title, int width, int height, float fps=60);

private:
    // Called each frame
    void step(float dt);
};
