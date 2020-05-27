#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "luigi"

namespace py = pybind11;

PYBIND11_MODULE(luigi, m)
{
    // --- Module --- //
    m.doc() = "Luigi 2D game engine";

    // --- Game --- //
    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def("start", &Game::start)
    ;


    // m.def("modify", &modify, "Multiply all entries of a list by 2.0");
    // m.def("test", &test, "tst");

    // py::class_<Test>(m, "Test")
    //     .def_readwrite("i", &Test::i)
    //     .def(py::init<int>())
    //     .def("p", &Test::p);
}
