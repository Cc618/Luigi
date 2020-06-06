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
#include "audio.h"

using namespace sf;
using namespace std;
using namespace lg;

// --- Screen Config --- //
ScreenConfig::ScreenConfig(const std::string& title, int width, int height, float fps, bool resizable)
    : title(title), width(width), height(height), fps(fps), resizable(resizable)
{
    Error::check(width > 0 && height > 0,  "Dimensions must be positive");
    Error::check(fps > 0, "FPS must be greater than 0");
}

// --- Game --- //
Game *Game::instance = nullptr;

void Game::set_clear_color(GLclampf r, GLclampf g, GLclampf b, GLclampf a) const
{
    glClearColor(r, g, b, a);
}

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

void Game::run(const std::function<void ()>& construct, const ScreenConfig& screen)
{
    // Target delta time
    float target_dt = 1.f / screen.fps;

    ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    ratio = (float)screen.width / (float)screen.height;
    win = new Window(VideoMode(screen.width, screen.height), screen.title,
        screen.resizable ? Style::Default : (Style::Default & (~Style::Resize)), settings);
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

    Clock clock;

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

void Game::exit()
{
    on_game = false;
}

void Game::start()
{
    // Init blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Init buffers
    VBO::create_square();

    lg::Shader::create_main();
}

void Game::update(float dt)
{
    Scene::current->update(dt);
}

void Game::draw()
{
    Camera::main->update(lg::Shader::set_main_cam);

    Scene::current->draw();
}

void Game::stop()
{
    Scene::current->stop();

    stop_audio();
}

void Game::set_scene(const std::string& name)
{
    if (on_game)
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

void Game::add_scene(const std::string& name, const std::function<void ()>& factory, const std::string& default_cam)
{
    Error::check(!on_game, "Can't create new scenes on game");

    // Register the scene, this object won't be destroyed
    new SceneFactory(name, factory, default_cam);
}

void Game::set_layer(const std::string& name)
{
    Error::check(Scene::current != nullptr, "No scene created, create a scene before setting layers");

    Scene::current->set_layer(name);
}

void Game::add_layer(const std::string& name, int z)
{
    Error::check(Scene::current != nullptr, "No scene created, create a scene before creating layers");

    Scene::current->add_layer(name, z);
}

Camera *Game::set_cam(const std::string& name)
{
    auto i = Camera::instances.find(name);

    // Ensure the name matches a camera
    Error::check(i != Camera::instances.end(), "Camera '" + name + "' doesn't exists (use create=True)");

    Camera::main = (*i).second;
    Camera::main->update_ratio();

    return Camera::main;
}

Camera *Game::add_cam(const std::string& name, float height)
{
    auto i = Camera::instances.find(name);

    // Ensure no camera with the same name exists
    Error::check(i == Camera::instances.end(), "Camera '" + name + "' already exists");

    // Create cam
    Camera *cam = new Camera(name, height);
    Camera::instances[name] = cam;

    return cam;
}

void Game::add(Entity *e)
{
    Error::check(Scene::current != nullptr, "No scene created, create a scene and a layer before adding entities");
    Scene::current->add(e);
}

lg::Sound *Game::add_sound(const std::string& name, const std::string& file)
{
    auto i = lg::Sound::instances.find(name);
    Error::check(i == lg::Sound::instances.end(), "The sound with name '" + name + "' already exists");

    // Create and register sound
    Sound *s = new Sound(name, file);
    Sound::instances[name] = s;

    return s;
}

lg::Music *Game::add_music(const std::string& name, const std::string& file)
{
    auto i = lg::Music::instances.find(name);
    Error::check(i == lg::Music::instances.end(), "The music with name '" + name + "' already exists");

    // Create and register sound
    Music *s = new Music(name, file);
    Music::instances[name] = s;

    return s;
}

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace std;

void bind_game(py::module &m)
{
    py::class_<ScreenConfig>(m, "ScreenConfig")
        .def(py::init<const std::string&, int, int, float, bool>(), py::arg("title"), py::arg("width"), py::arg("height"),
            py::arg("fps"), py::arg("resizable")=true)

        .doc() = "(**game**) Describes the window / the screen, used to run the game."
    ;

    py::class_<Game>(m, "Game")
        .def(py::init<>())

        // Audio
        .def("play", &Game::play, py::arg("name"),
            "Plays a sound or a music.")

        .def("set_fade_duration", &Game::set_fade_duration, py::arg("seconds"),
            "Sets the music transition duration.")

        .def("set_default_volume", &Game::set_default_volume, py::arg("sounds")=-1, py::arg("musics")=-1,
            R"(
                Sets the default volume for sounds and musics.

                .. note:: A value of -1 describes an unchanged default volume.
            )")

        // Inputs
        .def("pressed", &Game::pressed, py::arg("key"),
            "Whether a key is down.")

        .def("typed", &Game::typed, py::arg("key"),
            "Whether a key is typed (is down and was up the last frame).")

        .def("mouse_pressed", &Game::mouse_pressed, py::arg("button"),
            "Whether a mouse button is down.")

        .def("mouse_typed", &Game::mouse_typed, py::arg("button"),
            "Whether a mouse button is clicked.")

        // Shader
        .def("add_shader", &Game::add_shader, py::arg("name"), py::arg("vertex_file"), py::arg("fragment_file"), py::arg("uniforms"),
            R"(
                Creates a new shader.

                :arg uniforms: The list of all uniforms names.
            )")

        // Texture
        .def("add_texture", &Game::add_texture, py::arg("name"), py::arg("file"),
            "Creates a new texture.")

        .def("set_texture_filter", &Game::set_texture_filter, py::arg("filter"),
            R"(
                Sets the filter mode for textures created after this call.

                :param filter: The filter applied to the texture.

                **Filter types** :

                * pixel : No blur when scaling the texture (nearest).
                * blur : Blur when scaling the texture (linear).
            )")

        // Game
        .def("set_clear_color", &Game::set_clear_color,
            py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a")=1,
            "Sets the background color.")

        .def_readonly_static("instance", &Game::instance)

        .def("run", &Game::run, py::arg("construct"), py::arg("screen"),
            R"(
                Launches a window and runs the game.

                :param construct: This function is called when the game is initialized,
                    used to create scenes, cameras and set properties.
            )")

        .def("exit", &Game::exit,
            "Terminates the game and closes the window.")

        .def("set_scene", &Game::set_scene, py::arg("name"),
            R"(
                Sets a scene.

                .. note:: If this function is called in construct,
                    it sets the default scene (the first scene to create).
            )")

        .def("add_scene", &Game::add_scene, py::arg("name"), py::arg("factory")=nullptr, py::arg("default_cam")="main", py::keep_alive<1, 3>(),
            R"(
                Adds a scene factory.

                :param factory: This function is called when the scene is loaded, used to add entities to the scene.

                .. warning:: Don't add cameras to the scene in the factory function, add cameras
                    in the ``construct`` function when :func:`run` is called and set cameras' properties
                    in the ``factory`` function.
            )")

        .def("set_layer", &Game::set_layer, py::arg("name"),
            "Selects the layer to add entities.")

        .def("add_layer", &Game::add_layer, py::arg("name"), py::arg("z")=0,
            R"(
                Adds a layer.

                :param z: The z index of the layer, if a layer has a greater z than another layer,
                    then all drawable entities within this layer will be drawn above the entities
                    within the other layer.
            )")

        .def("set_cam", &Game::set_cam, py::arg("name"), py::return_value_policy::reference,
            R"(
                Sets the current camera.

                :return: The current camera, which is Camera.main.
            )")

        .def("add_cam", &Game::add_cam, py::arg("name"), py::arg("height")=100, py::return_value_policy::reference,
            "Adds a camera.")

        .def("add", &Game::add, py::arg("entity"), py::keep_alive<1, 2>(),
            "Adds an entity.")

        // TODO : Doc
        .def("add_sound", &Game::add_sound, py::arg("name"), py::arg("file"), py::return_value_policy::reference,
            "Adds a sound.")
        .def("add_music", &Game::add_music, py::arg("name"), py::arg("file"), py::return_value_policy::reference,
            "Adds a music.")

        .doc() = R"(
            (**game**) Handles the window and the game environment.
        )"
    ;
}
