#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "config.h"

using namespace std;
// using namespace lg;
namespace py = pybind11;

PYBIND11_MODULE(luigi, m)
{
    // Extern functions to have a faster build time
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

    // --- Module --- //
    m.doc() = "Luigi 2D game engine";
    m.attr("__version__") = LUIGI_VERSION_STR;

    // --- Camera --- //
    bind_camera(m);

    // --- Error --- //
    bind_error(m);

    // --- Entity --- //
    bind_entity(m);

    // --- Frame --- //
    bind_frame(m);

    // --- Game --- //
    bind_game(m);

    // --- Inputs --- //
    bind_inputs(m);

    // --- Maths --- //
    bind_maths(m);

    // --- Scene --- //
    bind_scene(m);

    // --- Shader --- //
    bind_shader(m);

    // --- Sprite --- //
    bind_sprite(m);

    // --- Texture --- //
    bind_texture(m);
}
