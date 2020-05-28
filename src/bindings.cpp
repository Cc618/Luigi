#include <pybind11/pybind11.h>
#include "luigi"

namespace py = pybind11;

void test() {
    Error::check(0 < 1, "OMG !");
}

#include <iostream>
using namespace std;
PYBIND11_MODULE(luigi, m)
{
    // --- Module --- //
    m.doc() = "Luigi 2D game engine";
    m.attr("version") = LUIGI_VERSION_STR;

    // --- Error --- //
    py::register_exception<Error>(m, "Error");

    m.def("test", &test);

    // --- Game --- //
    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def("start", &Game::start) // TODO : Args
        .doc() = "Handles the window and the game environment"
    ;
}
