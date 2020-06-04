#pragma once

// Handles the window and all Luigi functions

#include <string>
#include <functional>
#include <stack>
#include <SFML/Window.hpp>
#include "scene.h"
#include "camera.h"

namespace lg
{
    class Game
    {
        // --- Modules --- //
        // Game handles also global methods because
        // some arguments or returns must be keeped alive
    public: // Inputs
        // Whether a key is down
        bool pressed(const std::string& key) const;

        // Whether a key is down and was up the last frame
        bool typed(const std::string& key) const;

        void set_key_down(const sf::Keyboard::Key& key) const;

        bool mouse_pressed(const std::string& btn) const;

        bool mouse_typed(const std::string& btn) const;

        void update_inputs() const;

        void set_btn_down(const sf::Mouse::Button& btn) const;

    public: // Texture
        // Like Texture::create but without return
        // Mode :
        // - pixel : No blur when scaling the texture (nearest)
        // - blur : Blur when scaling the texture (linear)
        // TODO : returns Texture + chg doc
        void add_texture(const std::string& name, const std::string& file, const std::string& mode="pixel") const;

    public: // Shader
        // Like Shader::create but without return
        // TODO : returns Shader + chg doc
        void add_shader(const std::string& name, const std::string& vertex_file,
            const std::string& fragment_file, const std::list<std::string>& uniforms) const;


    public:
        void set_clear_color(GLclampf r, GLclampf g, GLclampf b, GLclampf a=1) const;

    public:
        static Game *instance;

    public:
        Game();
        virtual ~Game();

    public:
        // TODO : Use config (for fullscreen...)
        // Construct is called to create scenes, layers and
        void run(const std::function<void ()>& construct, const std::string& title, int width, int height, float fps=60);

        // TODO : Separate add 
        // Selects the current scene to add entities
        // * Used in the construct function pointer in run
        // * When a scene is changed, the next scene is automatically selected
        void set_scene(const std::string& name, bool create=false, const std::function<void ()>& factory=nullptr, const std::string& default_cam="main");

        // Like set_scene but for a layer within the current selected scene
        // * z is used only for creation
        void set_layer(const std::string& name, bool create=false, int index=0);

        // Adds an entity to the target scene and layer
        void add(Entity *e);

        // TODO : change_scene

        // TODO : Global scope
        // TODO : default option
        Camera *set_cam(const std::string& name, bool create=false, float height=100);

    public:
        // Aspect ratio
        float ratio;
        sf::Window *win;

    private:
        // After window creation
        // Returns the current scene
        void start();

        // Called each frame
        void update(float dt);
        void draw();

        // Before window destruction
        void stop();

    private:
        // Whether we are in the main loop
        bool on_game = false;

        // The scene loaded when the game starts
        std::string default_scene;

        // All events to call at the end of the frame
        std::stack<std::function<void ()>> events;
    };
}
