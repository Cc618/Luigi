#include "game.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

void Game::start(const std::string& title, int width, int height, float fps)
{
    sf::ContextSettings settings;
    // settings.depthBits = 24;
    // settings.stencilBits = 8;
    // settings.antialiasingLevel = 4;
    // settings.majorVersion = 3;
    // settings.minorVersion = 0;
    
    // create the window
    sf::Window window(sf::VideoMode(width, height), title, sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO : When time, dt
        step(0);

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }
}

#include <iostream>
void Game::step(float dt)
{
    // draw...
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(1, 0);
    glVertex2f(1, 1);
    glEnd();

    std::cout << "Frame\n";
}
