#include "entity.h"

using namespace lg;

void Entity::start()
{
    started = true;
}

void Entity::stop()
{
    started = false;
}

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "py_classes.h"

namespace py = pybind11;

void bind_entity(py::module &m)
{
    py::class_<Entity, PyEntity>(m, "Entity")
        .def(py::init<>())

        .def_readwrite("dead", &Entity::dead)

        // TMP
        .def("start", &Entity::start,
            "When all entities are created.")

        .def("update", &Entity::update, py::arg("dt"),
            R"(
            Called each frame.

            :param dt: The time elapsed since the last frame, in seconds.
            )")

        .def("draw", &Entity::draw,
            "Called each frame after update.")

        .def("stop", &Entity::stop,
            "When the entity is dead or the scene changes.")

        .doc() = R"(
            (**entity**) An entity is the most basic type for game objects.
            
            **Lifetime** :

            When all entities are created, the :func:`start` function is called.

            Then, :func:`update` and just after :func:`draw` are called until ``dead`` is set
            to true or the scene stops.

            After the last draw call, :func:`stop` is called.
        )"
    ;
}
