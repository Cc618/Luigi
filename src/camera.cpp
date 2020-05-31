#include "camera.h"
#include "error.h"

Camera *Camera::main = nullptr;

Mat3 *Camera::get_transform() const
{
    // TODO : Inv width and height
    return Mat3::create_transform(-x, -y, inv_width, inv_height, -rot, false);
}

Camera::Camera(float height, float x, float y, float rot)
    // TODO : Width with ratio
    : x(x), y(y), rot(rot)
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

void Camera::set_height(float val)
{
    Error::check(val > 0, "The height of the camera must be > 0");

    transform_changed = true;
    height = val;
    inv_height = 1.f / val;

    // TODO : width ratio
    width = val * 1;
    inv_width = 1.f / width;
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

