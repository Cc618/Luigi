#pragma once

// A screen with multiple layers of entities

#include <string>
#include <list>
#include "entity.h"
#include "layer.h"

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
    std::string name;

private:
    std::list<Layer*> layers;
};
