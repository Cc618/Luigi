#pragma once

// An image mapped to a camera

#include <GL/glew.h>
#include "entity.h"
#include "texture.h"

class Sprite : public Entity
{
public:
    static void init_sprites();

public:
    // TODO : Texture frame
    Sprite(const std::string& texture);
    // TODO : Update
    virtual ~Sprite() = default;

public:
    virtual void draw() override;

public:
    float x;
    float y;
    float rot;
    float width = 1;
    float height = 1;

private:
    // Ids
    static GLuint vbo_id;
    static int shader_id;

    // Uniform locations
    static int u_pos;
    static int u_transform;

private:
    void set_uniforms() const;

private:
    const Texture *texture;
};
