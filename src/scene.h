#pragma once

// A screen with multiple layers of entities

#include <string>
#include <list>
#include "entity.h"
#include "layer.h"

// TODO : Handle z index in layers
class Scene : public Entity
{
public:
    static Scene *current;

    // TODO : Map ?
    static std::list<Scene*> instances;

public:
    // Creates a new scene and registers it
    static Scene *create(const std::string& name);

    // Find a scene by name
    static Scene *find(const std::string& name);

public:
    virtual ~Scene() = default;

public:
    virtual void start() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void stop() override;

public:
    void add(Entity *e);

    // Like Game::set_layer
    void set_layer(const std::string& name, bool create=false, int z=0);

public:
    std::string name;
    Layer *selected_layer;
    std::list<Layer*> layers;

private:
    Scene(const std::string& name);
};
