#include "py_classes.h"

using namespace lg;

// --- PyEntity --- //
void PyEntity::start()
{
    PYBIND11_OVERLOAD(
        void,
        Entity,
        start,
    );
}

void PyEntity::update(float dt)
{
    PYBIND11_OVERLOAD(
        void,
        Entity,
        update,
        dt
    );
}

void PyEntity::draw()
{
    PYBIND11_OVERLOAD(
        void,
        Entity,
        draw,
    );
}

void PyEntity::stop()
{
    PYBIND11_OVERLOAD(
        void,
        Entity,
        stop,
    );
}
