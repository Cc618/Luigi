#pragma once

// An image mapped to a camera

#include <GL/glew.h>
#include "entity.h"
#include "texture.h"
#include "shader.h"

class Sprite : public Entity
{
public:
    static void init_sprites();

public:
    Sprite(const std::string& texture, const std::string& shader="main");
    // TODO : Update
    virtual ~Sprite() = default;

public:
    virtual void start() override;
    virtual void draw() override;

public:
    float x;
    float y;
    float rot;
    float width = 1;
    float height = 1;

private:
    // TMP
    // Ids
    static GLuint vbo_id;

private:
    void set_uniforms() const;

private:
    // TODO : Sprite sheet
    const Texture *texture;
    Shader *shader;
    // Uniform locations
    GLint u_pos, u_transform;
};
