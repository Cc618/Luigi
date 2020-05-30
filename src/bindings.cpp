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
        // TODO : Args
        .def("run", &Game::run) 
        .def("set_scene", &Game::set_scene, py::arg("name"), py::arg("create")=false)
        .def("set_layer", &Game::set_layer, py::arg("name"), py::arg("create")=false, py::arg("z")=0)
        .def("add", &Game::add, py::keep_alive<1, 2>())
        .doc() = "Handles the window and the game environment"
    ;

    // // --- Layer --- //
    // py::class_<Layer, Entity>(m, "Layer")
    //     .def(py::init<const string&, int>())
    //     .def_readwrite("name", &Layer::name)
    //     .def_readwrite("z", &Layer::z)
    //     .def("add", &Layer::add, py::arg("entity"), py::keep_alive<1, 2>())
    //     .doc() = "Gathers entities with same z index"
    // ;

    // // --- Scene --- //
    // py::class_<Scene, Entity>(m, "Scene")
    //     .def(py::init<const string&>())
    //     .def("add", &Scene::add, py::arg("entity"), py::keep_alive<1, 2>())
    //     .def_readwrite("name", &Scene::name)
    //     .doc() = "A screen with multiple layers of entities"
    // ;

    // --- Sprite --- //
    py::class_<Sprite, Entity, PySprite>(m, "Sprite")
        .def(py::init<float, float, float, float, float>())
        // TODO : Update
        .def_readwrite("x", &Sprite::x)

        // .def("start", py::overload_cast<>(&Entity::start))
        // .def("update", py::overload_cast<float>(&Entity::update), py::arg("dt"))
        // // .def("update", &Sprite::update, py::arg("dt"))
        // .def("draw", py::overload_cast<>(&Entity::draw))
        // .def("stop", py::overload_cast<>(&Entity::stop))
        
        .def("start", &Sprite::start)
        .def("update", &Sprite::update, py::arg("dt"))
        .def("draw", &Sprite::draw)
        .def("stop", &Sprite::stop)

        .doc() = "An image mapped to a camera"
    ;

    // py::class_<Sprite, PySprite>(m, "Sprite")
    //     .def(py::init<float, float, float, float, float>())
    //     // .def("update", &Sprite::update, py::arg("dt"))
    // ;

    // py::class_<Sprite, PySprite>(m, "Sprite")
    //     .def(py::init<float, float, float, float, float>())
    //     .def("update", &Sprite::update, py::arg("dt"))
    // ;

}
