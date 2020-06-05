#pragma once

// An image mapped to a camera

#include <string>
#include <GL/glew.h>
#include "entity.h"
#include "frame.h"
#include "shader.h"
#include "math.h"

namespace lg
{
    class Sprite : public Entity
    {
    public:
        // Takes the whole texture
        static Sprite *create(const std::string& texture, const std::string& shader="main");

    public:
        Sprite(const Frame *frame, const std::string& shader="main");
        virtual ~Sprite();

    public:
        virtual void start() override;
        virtual void update(float dt) override;
        virtual void draw() override;

    public:
        // Multiplies width and height by factor
        void scale(float factor);

        // Returns the AABB of the sprite
        // !!! Doesn't take in account the rotation
        Box rect() const;

    public:
        float x = 0;
        float y = 0;
        float rot = 0;
        float width = 1;
        float height = 1;
        Frame *frame;

    private:
        void set_uniforms() const;

    private:
        Shader *shader;
        
        // Uniform locations
        GLint u_transform;
        GLint u_tex_transform;
    };
}
