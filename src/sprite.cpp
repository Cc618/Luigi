#include "sprite.h"
#include <iostream>
#include <cmath>
#include "maths.h"
#include "buffers.h"

using namespace std;

Sprite::Sprite(const Frame& frame, const string& shader)
    : frame(frame), shader(Shader::get(shader))
{
    width = this->frame.first_region.width;
    height = this->frame.first_region.height;
}

void Sprite::start()
{
    u_transform = shader->get_uniform("transform");
}

void Sprite::update(float dt)
{
    frame.update(dt);
}















void Sprite::draw()
{
    // Send uniforms to the shader
    set_uniforms();

    // Set shader & texture
    shader->use();
    frame.texture->use();
    
    // // TODO : Not here ?
    auto tex = frame.get_transform();
    shader->set_mat3("tex_transform", tex);
    delete tex;

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

    // Flip y with -height
    auto m = Mat3::create_transform(x, y, width, -height, rot);
 
    shader->set_uniform_mat3(u_transform, m);

    delete m;
}
