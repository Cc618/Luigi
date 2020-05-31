#include "game.h"

#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include "error.h"
#include "sprite.h"
#include "shader.h"
#include "buffers.h"
#include "camera.h"

using namespace sf;
using namespace std;

Game *Game::instance = nullptr;

Game::Game()
{
    Error::check(instance == nullptr, "Only one game can be created");

    instance = this;
}

Game::~Game()
{
    // TODO : Reset static attributes
}

void Game::run(const std::function<void ()>& construct, const string &title, int width, int height, float fps)
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

    // Init glew
    GLenum err = glewInit();
    if (GLEW_OK != err)
        throw Error(string("GLEW init error : ") + (const char*)glewGetErrorString(err));

    // Init
    start();
    construct();

    // Set camera
    if (Camera::main == nullptr)
        Camera::main = new Camera(height);

    // Set first scene
    Error::check(Scene::instances.size() > 0, "No scene created");
    Scene::current = *Scene::instances.begin();
    Scene::current->start();

    bool on_game = true;
    auto clock = Clock();

    // Main loop
    try
    {
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
    }
    catch (Error &e)
    {
        cerr << "Uncaught luigi error: " << e.what() << endl;
    }
    catch (std::exception &e)
    {
        cerr << "Uncaught c++ error: " << e.what() << endl;
    }

    stop();
}

void Game::start()
{
    // Init buffers
    VBO::create_square();

    // Init blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    ::Shader::create_main();
}

void Game::update(float dt)
{
    Scene::current->update(dt);
}

void Game::draw()
{
    auto main_cam = Camera::main->get_transform();
    ::Shader::set_main_cam(main_cam);
    delete main_cam;

    Scene::current->draw();
}

void Game::stop()
{
    Scene::current->stop();

    // TODO : Stop all scenes and delete after
}

void Game::set_scene(const std::string& name, bool create)
{
    if (create)
        selected_scene = Scene::create(name);
    else
        selected_scene = Scene::find(name);
}

void Game::set_layer(const std::string& name, bool create, int z)
{
    Error::check(selected_scene != nullptr, "No scene created, create a scene before creating layers");

    selected_scene->set_layer(name, create, z);
}

void Game::add(Entity *e)
{
    Error::check(selected_scene != nullptr, "No scene created, create a scene and a layer before adding entities");
    selected_scene->add(e);
}

void Game::set_main_cam(float height, float x, float y, float rot) const
{
    if (Camera::main != nullptr)
        delete Camera::main;
    
    Camera::main = new Camera(height, x, y, rot);
}
