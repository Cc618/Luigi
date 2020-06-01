#pragma once

// An image mapped to a camera

#include <GL/glew.h>
#include "entity.h"
#include "frame.h"
#include "shader.h"

class Sprite : public Entity
{
public:
    Sprite(const Frame& frame, const std::string& shader="main");
    virtual ~Sprite() = default;

public:
    virtual void start() override;
    virtual void update(float dt) override;
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
    Frame frame;
    Shader *shader;
    
    // Uniform locations
    GLint u_transform;
    GLint u_tex_transform;
};
