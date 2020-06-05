#include "sprite.h"
#include <iostream>
#include <cmath>
#include "maths.h"
#include "buffers.h"

using namespace std;
using namespace lg;

Sprite *Sprite::create(const std::string& texture, const std::string& shader)
{
    return new Sprite(new Region(texture));
}

Sprite::Sprite(const Frame *frame, const string& shader)
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

Box Sprite::rect() const
{
    return Box(x - width * .5f, y - height * .5f, width, height);
}

void Sprite::set_uniforms() const
{
    shader->set_uniform_mat3(u_tex_transform, frame->get_transform());

    // Flip y with -height and -rot
    auto m = Mat3::create_srt(x, y, width, -height, -rot);
 
    shader->set_uniform_mat3(u_transform, m);

    delete m;
}

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "py_classes.h"

namespace py = pybind11;
using namespace std;

void bind_sprite(py::module &m)
{
    py::class_<Sprite, Entity, PyEntityChild<Sprite>>(m, "Sprite")
        .def(py::init<const Frame*, string>(), py::arg("frame"), py::arg("shader")="main")

        // TODO : Test return deletion (after scope)
        .def_static("create", &Sprite::create, py::arg("texture"), py::arg("shader")="main",
            "Creates a new sprite with the whole texture.")

        .def_readwrite("x", &Sprite::x)
        .def_readwrite("y", &Sprite::y)
        .def_readwrite("width", &Sprite::width)
        .def_readwrite("height", &Sprite::height)
        .def_readwrite("rot", &Sprite::rot)
        .def_readwrite("frame", &Sprite::frame)

        // TODO : Ref to inherited methods + try without these lines
        .def("start", &Sprite::start)
        .def("update", &Sprite::update, py::arg("dt"))
        .def("draw", &Sprite::draw)
        .def("stop", &Sprite::stop)

        .def("scale", &Sprite::scale, py::arg("factor"),
            "Multiplies width and height by a factor.")

        .def("rect", &Sprite::rect,
            R"(
                Returns the AABB of the sprite.
            
            .. warning:: Doesn't take in account the rotation.
            )")

        .doc() = R"(
            (**sprite**) A drawable entity, drawn with a shader, a texture region and a camera (the main camera).

            The sprite's position describes the center of the texture region.
        )"
    ;
}
