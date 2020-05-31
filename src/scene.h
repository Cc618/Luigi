#pragma once

// A screen with multiple layers of entities

#include <string>
#include <set>
#include "entity.h"
#include "layer.h"

class Scene : public Entity
{
public:
    static Scene *current;

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
    // A set is used to order layers by the z index
    std::set<Layer*, bool (*)(Layer*, Layer*)> layers;

private:
    Scene(const std::string& name);
};
