#pragma once

// Handles the window and the game environment

#include <string>
#include <functional>
#include "scene.h"
#include "camera.h"

class Game
{
public:
    static Game *instance;

public:
    Game();
    virtual ~Game();

public:
    // TODO : Use config (for fullscreen...)
    // Construct is called to create scenes, layers and
    void run(const std::function<void ()>& construct, const std::string& title, int width, int height, float fps=60);

    // Selects the current scene to add entities
    // * Used in the construct function pointer in run
    // * When a scene is changed, the next scene is automatically selected
    void set_scene(const std::string& name, bool create=false);

    // Like set_scene but for a layer within the current selected scene
    // * z is used only for creation
    void set_layer(const std::string& name, bool create=false, int index=0);

    // Adds an entity to the target scene and layer
    void add(Entity *e);

    // TODO : Update ?
    // Used in the constructor to create the main camera
    // void set_main_cam(float height, float x=0, float y=0, float rot=0) const;

    // TODO : change_scene
    // TODO : Change cam + Update cameras when changed

    Camera *add_cam(const std::string& name, float height, bool _default=false);

public:
    // Aspect ratio
    float ratio;

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
    // Selections
    Scene *selected_scene;
};
