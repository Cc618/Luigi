#include "game.h"

// TODO :
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include "error.h"

using namespace sf;
using namespace std;

Game *Game::instance = nullptr;

Game::Game()
{
    instance = this;
}

void Game::run(const string& title, int width, int height, float fps)
{
    Error::check(fps > 0.f, "FPS must be greater than 0");

    // Target delta time
    float target_dt = 1.f / fps;

    // TODO : Update
    ContextSettings settings;
    // settings.depthBits = 24;
    // settings.stencilBits = 8;
    // settings.antialiasingLevel = 4;
    // settings.majorVersion = 3;
    // settings.minorVersion = 0;

    Window win(VideoMode(width, height), title, Style::Default, settings);
    win.setVerticalSyncEnabled(true);
    win.setActive(true);

    start();

    bool on_game = true;
    auto clock = Clock();

    // Main loop
    while (on_game)
    {
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == Event::Closed)
                on_game = false;
            else if (event.type == Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        // Handle time
        // Delta time in seconds
        float dt = clock.getElapsedTime().asSeconds();

        // Wait for next frame
        if (dt < target_dt)
        {
            sleep(seconds(target_dt - dt));
            dt = target_dt;
        }

        clock.restart();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update(dt);
        draw();

        win.display();
    }

    stop();
}

void Game::start()
{

}

void Game::update(float dt)
{
}

void Game::draw()
{
    // draw...
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(1, 0);
    glVertex2f(1, 1);
    glEnd();
}

void Game::stop()
{

}
