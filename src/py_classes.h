#pragma once

// Some classes need wrappers (trampolines) to be inherited from python
// These classes start with Py and are binding with the c++ name (without py)

#include <pybind11/pybind11.h>
#include "entity.h"

namespace lg
{
    class PyEntity : public Entity
    {
    public:
        using Entity::Entity;

    public:
        void start() override;
        void update(float) override;
        void draw() override;
        void stop() override;
    };

    // To add a binding for an Entity, declare py::class_<T, Entity, PyEntityChild<T>>
    // With T the c++ class name
    template <class T=Entity>
    class PyEntityChild : public T
    {
    public:
        // Constructors
        using T::T;

    public:
        void start() override
        {
            PYBIND11_OVERLOAD(
                void,
                T,
                start,
            );
        }

        void update(float dt) override
        {
            PYBIND11_OVERLOAD(
                void,
                T,
                update,
                dt
            );
        }

        void draw() override
        {
            PYBIND11_OVERLOAD(
                void,
                T,
                draw,
            );
        }

        void stop() override
        {
            PYBIND11_OVERLOAD(
                void,
                T,
                stop,
            );
        }
    };
}
