#pragma once

// A screen with multiple layers of entities

#include <string>
#include <list>
#include "entity.h"
#include "layer.h"

// TMP
#include <iostream>
#include <vector>
class TstScene
{
public:
    TstScene()
    {
        std::cout << "TstScene()\n";
    }
    ~TstScene()
    {
        std::cout << "~TstScene()\n";
    }

    void add(Entity *e)
    {
        std::cout << "TstScene::add\n";
        entities.push_back(e);
    }

    void update(float dt)
    {
        std::cout << "TstScene::update\n";

        for (auto e : entities)
            e->update(dt);
    }

    std::vector<Entity*> entities;
};


class Scene : public Entity
{
public:
    static Scene *current;

public:
    Scene(const std::string& name);
    virtual ~Scene();

public:
    virtual void start() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void stop() override;

public:
    // TODO : Layer arg
    void add(Entity *e);
    // TODO :
    // void add_layer(Layer *l);

public:
    std::string name;

private:
    std::list<Layer*> layers;
};
