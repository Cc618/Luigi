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

    // --- Camera --- //
    py::class_<Camera>(m, "Camera")
        .def_readwrite_static("main", &Camera::main)

        // TODO : Properties to avoid updating each frame
        .def_property("x", &Camera::get_x, &Camera::set_x)
        .def_property("y", &Camera::get_y, &Camera::set_y)
        .def_property_readonly("width", &Camera::get_width)
        .def_property("height", &Camera::get_height, &Camera::set_height)
        .def_property("rot", &Camera::get_rot, &Camera::set_rot)
        .def("set", &Camera::set, py::arg("x")=0, py::arg("y")=0, py::arg("height")=-1, py::arg("rot")=-1,
            py::doc("If height or rot == -1, then height / rot is not changed"))

        .doc() = "A 2D camera describing a view"
    ;

    // --- Entity --- //
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

    // --- Frame --- //
    py::class_<Frame>(m, "Frame")
        // TODO : Other constructors
        .def(py::init<const string&, const std::vector<Box>&, float>())
        
        .doc() = "Handles the window and the game environment"
    ;

    // --- Game --- //
    py::class_<Game>(m, "Game")
        .def(py::init<>())

        .def_readonly_static("instance", &Game::instance)

        .def("run", &Game::run) // TODO : Args
        .def("set_scene", &Game::set_scene, py::arg("name"), py::arg("create")=false)
        .def("set_layer", &Game::set_layer, py::arg("name"), py::arg("create")=false, py::arg("z")=0)
        .def("add", &Game::add, py::arg("entity"), py::keep_alive<1, 2>())
        // TMP .def("set_main_cam", &Game::set_main_cam, py::arg("height"), py::arg("x")=0, py::arg("y")=0, py::arg("rot")=0)
        .def("set_cam", &Game::set_cam, py::arg("name"), py::arg("create")=false, py::arg("height")=100, py::arg("default")=true, py::return_value_policy::reference)

        .doc() = "Handles the window and the game environment"
    ;

    // --- Input --- //
    m.def("pressed", &pressed, py::arg("name"));

    // --- Maths --- //
    py::class_<Mat3>(m, "Mat3")
        .def_static("create_tsr", &Mat3::create_tsr, py::arg("x"), py::arg("y"), py::arg("width")=1, py::arg("height")=1, py::arg("rot")=0)
        .def_static("create_rst", &Mat3::create_rst, py::arg("x"), py::arg("y"), py::arg("width")=1, py::arg("height")=1, py::arg("rot")=0)
        .def_static("create_srt", &Mat3::create_srt, py::arg("x"), py::arg("y"), py::arg("width")=1, py::arg("height")=1, py::arg("rot")=0)
        .def_static("create_id", &Mat3::create_id)

        .doc() = "A 3x3 matrix"
    ;

    py::class_<Box>(m, "Box")
        // TODO : args
        .def(py::init<GLfloat, GLfloat, GLfloat, GLfloat>())

        .def_readwrite("x", &Box::x)
        .def_readwrite("y", &Box::y)
        .def_readwrite("width", &Box::width)
        .def_readwrite("height", &Box::height)

        .doc() = "A 2D region"
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
        .def(py::init<const Frame&, string>(), py::arg("frame"), py::arg("shader")="main")

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
    m.def("new_texture", &new_texture, py::arg("name"), py::arg("file"), py::arg("mode")="pixel",
        py::doc(R"(Mode :
- pixel : No blur when scaling the texture (nearest)
- blur : Blur when scaling the texture (linear))"));
}
