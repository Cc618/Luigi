#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "config.h"

using namespace std;
namespace py = pybind11;

PYBIND11_MODULE(luigi, m)
{
    // Extern functions to have a faster build time
    extern void bind_audio(py::module &m);
    extern void bind_camera(py::module &m);
    extern void bind_entity(py::module &m);
    extern void bind_error(py::module &m);
    extern void bind_frame(py::module &m);
    extern void bind_game(py::module &m);
    extern void bind_inputs(py::module &m);
    extern void bind_maths(py::module &m);
    extern void bind_texture(py::module &m);
    extern void bind_scene(py::module &m);
    extern void bind_shader(py::module &m);
    extern void bind_sprite(py::module &m);

    m.doc() = "Luigi 2D game engine";
    m.attr("__version__") = LUIGI_VERSION_STR;

    bind_audio(m);
    bind_camera(m);
    bind_error(m);
    bind_entity(m);
    bind_frame(m);
    bind_game(m);
    bind_inputs(m);
    bind_maths(m);
    bind_scene(m);
    bind_shader(m);
    bind_sprite(m);
    bind_texture(m);
}
