#pragma once

// A screen with multiple layers of entities

#include <string>
#include <list>
#include "entity.h"
#include "layer.h"

// // TMP
// #include <iostream>
// #include <vector>
// class TstScene
// {
// public:
//     TstScene()
//     {
//         std::cout << "TstScene()\n";
//     }
//     ~TstScene()
//     {
//         std::cout << "~TstScene()\n";
//     }

//     void add(Entity *e)
//     {
//         std::cout << "TstScene::add\n";
//         entities.push_back(e);
//     }

//     void update(float dt)
//     {
//         std::cout << "TstScene::update\n";

//         for (auto e : entities)
//             e->update(dt);
//     }

//     std::vector<Entity*> entities;
// };


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
    virtual ~Scene();

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
    Scene(const std::string& name);};
