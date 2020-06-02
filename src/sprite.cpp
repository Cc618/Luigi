#include "sprite.h"
#include <iostream>
#include <cmath>
#include "maths.h"
#include "buffers.h"

using namespace std;

Sprite *Sprite::create(const std::string& texture, const std::string& shader)
{
    const Texture *tex = Texture::get(texture);

    auto frame = new Region(tex, Box(0, 0, tex->width, tex->height));

    auto s = new Sprite(frame);

    delete frame;

    return s;
}

Sprite::Sprite(const Region *frame, const string& shader)
    : frame(frame->copy()), shader(Shader::get(shader))
{
    width = this->frame->rect.width;
    height = this->frame->rect.height;
}

Sprite::~Sprite()
{
    delete frame;
}

void Sprite::start()
{
    u_transform = shader->get_uniform("transform");
    u_tex_transform = shader->get_uniform("tex_transform");
}

void Sprite::update(float dt)
{
    frame->update(dt);
}

void Sprite::draw()
{
    // Set shader & texture
    frame->texture->use();

    // Send uniforms to the shader
    shader->use();
    set_uniforms();

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
    auto tex = frame->get_transform();
    shader->set_uniform_mat3(u_tex_transform, tex);
    delete tex;

    // Flip y with -height
    auto m = Mat3::create_srt(x, y, width, -height, rot);
 
    shader->set_uniform_mat3(u_transform, m);

    delete m;
}
