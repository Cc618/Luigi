#include "py_classes.h"

#include <pybind11/pybind11.h>

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

void PySprite::start()
{
    PYBIND11_OVERLOAD(
        void,
        Sprite,
        start,
    );
}

void PySprite::update(float dt)
{
    PYBIND11_OVERLOAD(
        void,
        Sprite,
        update,
        dt
    );
}

void PySprite::draw()
{
    PYBIND11_OVERLOAD(
        void,
        Sprite,
        draw,
    );
}

void PySprite::stop()
{
    PYBIND11_OVERLOAD(
        void,
        Sprite,
        stop,
    );
}
