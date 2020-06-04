#include "game.h"

#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include "error.h"
#include "sprite.h"
#include "shader.h"
#include "buffers.h"
#include "camera.h"
#include "inputs.h"

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
    if (win)
        delete win;
}

void Game::run(const std::function<void ()>& construct, const string &title, int width, int height, float fps)
{
    Error::check(fps > 0.f, "FPS must be greater than 0");

    // Target delta time
    float target_dt = 1.f / fps;

    ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    ratio = (float)width / (float)height;
    win = new Window(VideoMode(width, height), title, Style::Default, settings);
    win->setVerticalSyncEnabled(true);
    win->setKeyRepeatEnabled(false);
    win->setActive(true);

    // Init glew
    GLenum err = glewInit();
    if (GLEW_OK != err)
        throw Error(string("GLEW init error : ") + (const char*)glewGetErrorString(err));

    // Init
    start();
    construct();

    // Set first scene
    Error::check(SceneFactory::instances.size() > 0, "No scene created");

    auto first_scene = SceneFactory::instances.find(default_scene);
    Error::check(first_scene != SceneFactory::instances.end(), "The default scene '" + default_scene + "' is not found");

    Scene::current = (*first_scene).second->spawn();


    auto clock = Clock();

    // Main loop
    try
    {
        on_game = true;
        while (on_game)
        {
            update_inputs();

            Event event;
            while (win->pollEvent(event))
            {
                if (event.type == Event::Closed)
                    on_game = false;
                else if (event.type == Event::Resized)
                {
                    glViewport(0, 0, event.size.width, event.size.height);
                    ratio = (float)event.size.width / (float)event.size.height;

                    Camera::main->update_ratio();
                }
                else if (event.type == Event::KeyPressed)
                    set_key_down(event.key.code);
                else if (event.type == Event::MouseButtonPressed)
                    set_btn_down(event.mouseButton.button);
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

            glClear(GL_COLOR_BUFFER_BIT);

            update(dt);
            draw();

            win->display();

            // Parse all events
            while (!events.empty())
            {
                // Call the last event
                events.top()();
                events.pop();
            }
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

    on_game = false;

    stop();
}

void Game::start()
{
    // Init blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Init buffers
    VBO::create_square();

    ::Shader::create_main();
}

void Game::update(float dt)
{
    Scene::current->update(dt);
}

void Game::draw()
{
    Camera::main->update(::Shader::set_main_cam);

    Scene::current->draw();
}

void Game::stop()
{
    Scene::current->stop();
}

void Game::set_scene(const std::string& name, bool create, const std::function<void ()>& factory, const std::string& default_cam)
{
    if (create)
    {
        Error::check(!on_game, "Can't create new scenes on game");

        // Register the scene, this scene won't be destroyed
        new SceneFactory(name, factory, default_cam);
    }
    else if (on_game)
    {
        // Change scene at the end of the frame
        events.push([name](){
            Scene::current->stop();
            delete Scene::current;

            auto scn = SceneFactory::instances.find(name);
            Error::check(scn != SceneFactory::instances.end(), "The scene with name '" + name + "' is not found");
            Scene::current = (*scn).second->spawn();
        });
    }
    else
        default_scene = name;
}

void Game::set_layer(const std::string& name, bool create, int z)
{
    Error::check(Scene::current != nullptr, "No scene created, create a scene before creating layers");

    Scene::current->set_layer(name, create, z);
}

void Game::add(Entity *e)
{
    Error::check(Scene::current != nullptr, "No scene created, create a scene and a layer before adding entities");
    Scene::current->add(e);
}

Camera *Game::set_cam(const std::string& name, bool create, float height)
{
    auto i = Camera::instances.find(name);

    if (create)
    {
        // Ensure no camera with the same name exists
        Error::check(i == Camera::instances.end(), "Camera '" + name + "' already exists");

        // Create cam
        Camera *cam = new Camera(name, height);
        Camera::instances[name] = cam;

        return cam;
    }

    // Ensure the name matches a camera
    Error::check(i != Camera::instances.end(), "Camera '" + name + "' doesn't exists (use create=True)");

    Camera::main = (*i).second;
    Camera::main->update_ratio();

    return Camera::main;
}

// --- Bindings --- //
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace std;

void bind_game(py::module &m)
{
    py::class_<Game>(m, "Game")
        .def(py::init<>())

        .def_readonly_static("instance", &Game::instance)

        // TODO : Args
        .def("run", &Game::run, py::arg("construct"), py::arg("title"), py::arg("width"), py::arg("height"), py::arg("fps")=60,
            R"(
                Launches a window and runs the game.

                :param construct: This function is called when the game is initialized,
                    used to create scenes, cameras and set properties.                   
            )")

        .def("set_scene", &Game::set_scene, py::arg("name"), py::arg("create")=false, py::arg("factory")=nullptr, py::arg("default_cam")="main", py::keep_alive<1, 4>(),
            R"(
                Sets / adds a scene.
            
                :param factory: This function is called when the scene is loaded, used to add entities to the scene.

                .. warning:: Don't add cameras to the scene in the factory function, add cameras
                    in the ``construct`` function when :func:`run` is called and set cameras' properties
                    in the ``factory`` function.
            )")
        
        .def("set_layer", &Game::set_layer, py::arg("name"), py::arg("create")=false, py::arg("z")=0,
            R"(
                Sets / adds a layer.

                :param z: The z index of the layer, if a layer has a greater z than another layer,
                    then all drawable entities within this layer will be drawn above the entities
                    within the other layer.
            )")
        
        .def("add", &Game::add, py::arg("entity"), py::keep_alive<1, 2>(),
            "Adds an entity.")
        
        .def("set_cam", &Game::set_cam, py::arg("name"), py::arg("create")=false, py::arg("height")=100, py::return_value_policy::reference,
            "Sets / adds a camera.")

        .doc() = R"(
        (**game**) Handles the window and the game environment.
        )"
    ;

    // TODO : mv
    // game_fun
    m.def("set_clear_color", &set_clear_color,
        py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a")=1,
        "Sets the background color.");
}
