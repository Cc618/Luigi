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


    // --- Sprite --- //
    py::class_<Sprite, Entity, PyEntityChild<Sprite>>(m, "Sprite")
        .def(py::init<std::string>(), py::arg("texture"))

        .def_readwrite("x", &Sprite::x)
        .def_readwrite("y", &Sprite::y)
        .def_readwrite("width", &Sprite::width)
        .def_readwrite("height", &Sprite::height)
        .def_readwrite("rot", &Sprite::rot)

        .def("start", &Sprite::start)
        .def("update", &Sprite::update, py::arg("dt"))
        .def("draw", &Sprite::draw)
        .def("stop", &Sprite::stop)

        .doc() = "An image mapped to a camera"
    ;

    // --- Texture --- //
    m.def("new_texture", &new_texture, py::arg("name"), py::arg("file"));
}
