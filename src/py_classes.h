#pragma once

// Some classes need wrappers (trampolines) to be inherited from python
// These classes start with Py and are binding with the c++ name (without py)

#include "luigi.h"

class PyEntity : public Entity
{
public:
    using Entity::Entity;

public:
    void start() override;
    void update(float) override;
    void draw() override;
    void stop() override;
};

class PySprite : public Sprite
{
public:
    using Sprite::Sprite;

public:
    void start() override;
    void update(float) override;
    void draw() override;
    void stop() override;
};
