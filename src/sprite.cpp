#include "sprite.h"
#include <iostream>
#include <cmath>
#include "maths.h"
#include "buffers.h"

using namespace std;

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

    // Bind buffer and draw it
    VBO::square->use();
    VBO::use_square(VBO::square);

    VBO::square->draw();
    
    VBO::unuse_square();
}

void Sprite::scale(float factor)
{
    width *= factor;
    height *= factor;
}

void Sprite::set_uniforms() const
{
    shader->use();

    auto m = Mat3::create_transform(x, y, width, height, rot);
 
    shader->set_uniform_mat3(u_transform, m);

    delete m;
}
