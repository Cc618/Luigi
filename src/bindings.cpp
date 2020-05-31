#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "py_classes.h"
#include "luigi"

using namespace std;
namespace py = pybind11;

PYBIND11_MODULE(luigi, m)
{
    // --- Module --- //
    m.doc() = "Luigi 2D game engine";
    m.attr("version") = LUIGI_VERSION_STR;

    // --- Entity --- //
    // py::class_<Entity>(m, "Entity")
    //     .def(py::init<>())
    //     .def("start", &Entity::start)
    //     .def("update", &Entity::update, py::arg("dt"))
    //     .def("draw", &Entity::draw)
    //     .def("stop", &Entity::stop)
    //     .doc() = "An entity is the most basic (abstract) type for game objects"
    // ;

    py::class_<Entity, PyEntity>(m, "Entity")
        .def(py::init<>())

        .def_readwrite("dead", &Entity::dead)

        .def("start", &Entity::start)
        .def("update", &Entity::update, py::arg("dt"))
        .def("draw", &Entity::draw)
        .def("stop", &Entity::stop)

        .doc() = "An entity is the most basic (abstract) type for game objects"
    ;

    // --- Error --- //
    py::register_exception<Error>(m, "Error");

    // --- Game --- //
    py::class_<Game>(m, "Game")
        .def(py::init<>())

        .def_readonly_static("instance", &Game::instance)

        .def("run", &Game::run) // TODO : Args
        .def("set_scene", &Game::set_scene, py::arg("name"), py::arg("create")=false)
        .def("set_layer", &Game::set_layer, py::arg("name"), py::arg("create")=false, py::arg("z")=0)
        .def("add", &Game::add, py::keep_alive<1, 2>())

        .doc() = "Handles the window and the game environment"
    ;

    // --- Input --- //
    m.def("pressed", &pressed, py::arg("name"));

    // --- Maths --- //
    py::class_<Mat3>(m, "Mat3")
        .def_static("create_transform", &Mat3::create_transform, py::arg("x"), py::arg("y"), py::arg("width")=1, py::arg("height")=1, py::arg("rot")=0)
        .def_static("create_id", &Mat3::create_id)

        .doc() = "A 3x3 matrix"
    ;

    // --- Shader --- //
    m.def("new_shader", &new_shader, py::arg("name"), py::arg("vertex_file"), py::arg("fragment_file"), py::arg("uniforms"));

    py::class_<Shader>(m, "Shader")
        .def_static("get", &Shader::get, py::arg("name"))

        .def("get_uniform", &Shader::get_uniform, py::arg("name"))
        .def("set_uniform_1f", &Shader::set_uniform_1f, py::arg("id"), py::arg("x"))
        .def("set_uniform_2f", &Shader::set_uniform_2f, py::arg("id"), py::arg("x"), py::arg("y"))
        .def("set_uniform_3f", &Shader::set_uniform_3f, py::arg("id"), py::arg("x"), py::arg("y"), py::arg("z"))
        .def("set_uniform_4f", &Shader::set_uniform_4f, py::arg("id"), py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a"))
        .def("set_uniform_mat3", &Shader::set_uniform_mat3, py::arg("id"), py::arg("mat"))
        .def("set_1f", &Shader::set_1f, py::arg("name"), py::arg("x"))
        .def("set_2f", &Shader::set_2f, py::arg("name"), py::arg("x"), py::arg("y"))
        .def("set_3f", &Shader::set_3f, py::arg("name"), py::arg("x"), py::arg("y"), py::arg("z"))
        .def("set_4f", &Shader::set_4f, py::arg("name"), py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a"))
        .def("set_mat3", &Shader::set_mat3, py::arg("name"), py::arg("mat"))

        .doc() = "A GPU program"
    ;

    // --- Sprite --- //
    py::class_<Sprite, Entity, PyEntityChild<Sprite>>(m, "Sprite")
        .def(py::init<string, string>(), py::arg("texture"), py::arg("shader")="main")

        .def_readwrite("x", &Sprite::x)
        .def_readwrite("y", &Sprite::y)
        .def_readwrite("width", &Sprite::width)
        .def_readwrite("height", &Sprite::height)
        .def_readwrite("rot", &Sprite::rot)

        .def("start", &Sprite::start)
        .def("update", &Sprite::update, py::arg("dt"))
        .def("draw", &Sprite::draw)
        .def("stop", &Sprite::stop)

        .def("scale", &Sprite::scale, py::arg("factor"))

        .doc() = "An image mapped to a camera"
    ;

    // --- Texture --- //
    m.def("new_texture", &new_texture, py::arg("name"), py::arg("file"));
}
