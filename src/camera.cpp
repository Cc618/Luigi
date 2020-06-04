#include "camera.h"
#include "error.h"
#include "game.h"

using namespace std;
using namespace lg;

Camera *Camera::main = nullptr;

unordered_map<string, Camera*> Camera::instances;

Mat3 *Camera::get_transform() const
{
    return Mat3::create_trs(-x, -y, inv_half_width, inv_half_height, rot);
}

Camera::Camera(const string& name, float height, float x, float y, float rot)
    : name(name), x(x), y(y), rot(rot)
{
    set_height(height);
}

void Camera::update(void (*set_transform)(const Mat3 *transform))
{
    if (!transform_changed)
        return;

    // Update shaders
    auto t = get_transform();
    set_transform(t);
    delete t;

    transform_changed = false;
}

float Camera::get_x() const
{
    return x;
}

float Camera::get_y() const
{
    return y;
}

float Camera::get_width() const
{
    return width;
}

float Camera::get_height() const
{
    return height;
}

float Camera::get_rot() const
{
    return rot;
}

void Camera::set(float x, float y, float height, float rot)
{
    if (height > 0)
        set_height(height);
 
    if (rot > 0)
        set_rot(rot);
    
    set_x(x);
    set_y(y);
}

void Camera::set_height(float val)
{
    Error::check(val > 0, "The height of the camera must be > 0");

    height = val;
    inv_half_height = 2.f / val;

    // transform_changed is set to true here
    update_ratio();
}

void Camera::set_x(float val)
{
    transform_changed = true;
    x = val;
}

void Camera::set_y(float val)
{
    transform_changed = true;
    y = val;
}

void Camera::set_rot(float val)
{
    transform_changed = true;
    rot = val;
}

void Camera::update_ratio()
{
    transform_changed = true;

    width = height * Game::instance->ratio;
    inv_half_width = 2.f / width;
}

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void bind_camera(py::module &m)
{
    py::class_<Camera>(m, "Camera")
        .def_readwrite_static("main", &Camera::main)

        .def_readwrite("name", &Camera::name)

        .def_property("x", &Camera::get_x, &Camera::set_x)
        .def_property("y", &Camera::get_y, &Camera::set_y)
        .def_property_readonly("width", &Camera::get_width)
        .def_property("height", &Camera::get_height, &Camera::set_height)
        .def_property("rot", &Camera::get_rot, &Camera::set_rot)

        .def("set", &Camera::set, py::arg("x")=0, py::arg("y")=0, py::arg("height")=-1, py::arg("rot")=-1,
            R"(
            Sets the properties of the camera.

            .. note::
                If height or rot == -1, then height / rot is not changed.
            )")

        .doc() = R"(
            (**camera**) A 2D camera describing a view.
        
        .. note ::
            The width of the camera is calculated by its height and the screen aspect ratio.
        )"
    ;
}
