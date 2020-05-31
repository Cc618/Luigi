#pragma once

// An image mapped to a camera

#include <GL/glew.h>
#include "entity.h"
#include "texture.h"
#include "shader.h"

class Sprite : public Entity
{
public:
    Sprite(const std::string& texture, const std::string& shader="main");
    virtual ~Sprite() = default;

public:
    virtual void start() override;
    virtual void draw() override;

public:
    // Multiplies width and height by factor
    void scale(float factor);

public:
    float x = 0;
    float y = 0;
    float rot = 0;
    float width = 1;
    float height = 1;

private:
    void set_uniforms() const;

private:
    // TODO : Sprite sheet
    const Texture *texture;
    Shader *shader;
    
    // Uniform locations
    GLint u_transform;
};
