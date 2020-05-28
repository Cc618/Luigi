#include <pybind11/pybind11.h>
#include "luigi"

namespace py = pybind11;

#include <iostream>
using namespace std;
PYBIND11_MODULE(luigi, m)
{
    // --- Module --- //
    m.doc() = "Luigi 2D game engine";
    m.attr("version") = LUIGI_VERSION_STR;

    // --- Error --- //
    py::register_exception<Error>(m, "Error");

    // --- Game --- //
    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def("run", &Game::run) // TODO : Args
        .doc() = "Handles the window and the game environment"
    ;
}
