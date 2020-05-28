#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "luigi"
namespace py = pybind11;

#include <iostream>
using namespace std;
PYBIND11_MODULE(luigi, m)
{
    // --- Module --- //
    m.doc() = "Luigi 2D game engine";
    m.attr("version") = LUIGI_VERSION_STR;

    // --- Entity --- //
    py::class_<Entity>(m, "Entity")
        .def(py::init<>())
        .doc() = "An entity is the most basic (abstract) type for game objects"
    ;

    // --- Error --- //
    py::register_exception<Error>(m, "Error");

    // --- Game --- //
    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def("run", &Game::run) // TODO : Args
        .doc() = "Handles the window and the game environment"
    ;

    // --- Layer --- //
    py::class_<Layer, Entity>(m, "Layer")
        .def(py::init<const string&, const list<Entity*>&>())
        .def_readwrite("name", &Layer::name)
        .def_readwrite("z", &Layer::z)
        .doc() = "Gathers entities with same z index"
    ;

    // --- Scene --- //
    py::class_<Scene, Entity>(m, "Scene")
        // TODO :
        // .def_readonly_static("current", &Scene::current)
        .def(py::init<const string&, const list<Layer*>&>())
        .def_readwrite("name", &Scene::name)
        .doc() = "A screen with multiple layers of entities"
    ;
}
