#include "sprite.h"
#include <iostream>
#include <cmath>

using namespace std;

constexpr size_t SPRITE_VERTEX_SIZE = 4;

// TODO : In math
void genTransform(float *data, float rot, float width, float height)
{
    data[0] = width * cosf(rot);
    data[1] = width * sinf(rot);
    data[2] = -sinf(rot) * height;
    data[3] = height * cosf(rot);
}

// Ids
GLuint Sprite::vbo_id;

void Sprite::init_sprites()
{
    // Generate VBO //
    // x, y, u, v
    float vertices[6 * SPRITE_VERTEX_SIZE] = {
        -.5f,   -.5f,   0.f,  0.f,
        0.5f,   -.5f,   1.f,  0.f,
        0.5f,   0.5f,   1.f,  1.f,
        0.5f,   0.5f,   1.f,  1.f,
        -.5f,   -.5f,   0.f,  0.f,
        -.5f,   0.5f,   0.f,  1.f,
    };

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, 6 * SPRITE_VERTEX_SIZE * sizeof(float), vertices, GL_STATIC_DRAW);
}

Sprite::Sprite(const string& texture, const string& shader)
    : texture(Texture::get(texture)), shader(Shader::get(shader))
{
    width = this->texture->width;
    height = this->texture->height;
}

void Sprite::start()
{
    u_pos = shader->get_uniform("pos");
    u_transform = shader->get_uniform("transform");
}

void Sprite::draw()
{
    // Send uniforms to the shader
    set_uniforms();

    // Set shader & texture
    shader->bind();
    texture->bind();

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void Sprite::set_uniforms() const
{
    shader->bind();

    // TODO : One transformation matrix

    // Position
    shader->set_uniform_2f(u_pos, x, y);

    // Transform
    // TODO : See flip y for cam or for sprite
    float transform[4];
    genTransform(transform, rot, width, -height);
    shader->set_uniform_mat2(u_transform, transform[0], transform[1], transform[2], transform[3]);
}
