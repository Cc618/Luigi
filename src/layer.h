#pragma once

// Gathers entities with same z index

#include <string>
#include <list>
#include "entity.h"

class Layer : public Entity
{
public:
    Layer(const std::string& name, int z=0);
    virtual ~Layer();
    
public:
    virtual void start() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void stop() override;

public:
    std::string name;
    // To draw over layers with lesser z indices
    int z;

private:
    std::list<Entity*> entities;
};
