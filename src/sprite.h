#pragma once

// An image mapped to a camera

#include <GL/glew.h>
#include "entity.h"

class Sprite : public Entity
{
public:
    static void init_sprites();
    static void stop_sprites();

public:
    Sprite(float x=0, float y=0, float rot=0, float width=1, float height=1);
    // TODO : Update
    virtual ~Sprite() = default;

public:
    virtual void draw() override;

public:
    float x;
    float y;
    float rot;
    float width = 1.f;
    float height = 1.f;

private:
    void set_uniforms() const;

private:
    // Ids
    static GLuint vbo_id;
    static int shader_id;

    // Uniform locations
    static int u_pos;
    static int u_transform;
};