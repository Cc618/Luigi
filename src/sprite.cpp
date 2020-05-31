#include "sprite.h"
#include <iostream>
#include <cmath>
#include "maths.h"

using namespace std;

constexpr size_t SPRITE_VERTEX_SIZE = 5;

// TMP : rm
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
        -.5f,   -.5f,   1,  0.f,  0.f,
        0.5f,   -.5f,   1,  1.f,  0.f,
        0.5f,   0.5f,   1,  1.f,  1.f,
        0.5f,   0.5f,   1,  1.f,  1.f,
        -.5f,   -.5f,   1,  0.f,  0.f,
        -.5f,   0.5f,   1,  0.f,  1.f,
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
    u_transform = shader->get_uniform("transform");
}

void Sprite::draw()
{
    // Send uniforms to the shader
    set_uniforms();

    // Set shader & texture
    shader->use();
    texture->use();

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SPRITE_VERTEX_SIZE * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, SPRITE_VERTEX_SIZE * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void Sprite::set_uniforms() const
{
    shader->use();

    // auto m = Mat3::create_id();
    auto m = Mat3::create_transform(x, y, width, height, rot);
 
    shader->set_uniform_mat3(u_transform, m);

    delete m;
}
