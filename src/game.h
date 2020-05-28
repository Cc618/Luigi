#pragma once

// Handles the window and the game environment

#include <string>

class Game
{
public:
    static Game *instance;

public:
    Game();
    virtual ~Game() = default;

public:
    // TODO : Use config (for fullscreen...)
    void run(const std::string& title, int width, int height, float fps=60);

private:
    // After window creation
    void start();
    
    // Called each frame
    void update(float dt);
    void draw();

    // Before window destruction
    void stop();
};
