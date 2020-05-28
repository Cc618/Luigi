#pragma once

// An entity is the most basic (abstract) type for game objects
// Provides main lifetime functions

class Entity
{
public:
    Entity() = default;
    virtual ~Entity() = default;

public:
    virtual void start()
    {}

    // Called each frame
    virtual void update(float dt)
    {}
    virtual void draw()
    {}

    virtual void stop()
    {}

public:
    // Change this to true to remove it
    bool dead;
};
