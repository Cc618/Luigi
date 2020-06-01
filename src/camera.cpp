#include "camera.h"
#include "error.h"
#include "game.h"

Camera *Camera::main = nullptr;

Mat3 *Camera::get_transform() const
{
    return Mat3::create_tsr(-x, -y, inv_half_width, inv_half_height, -rot);
}

Camera::Camera(float height, float x, float y, float rot)
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














#include <iostream>

void Camera::set_height(float val)
{
    Error::check(val > 0, "The height of the camera must be > 0");

    transform_changed = true;
    height = val;
    inv_half_height = 2.f / val;
std::cout << height << std::endl;

    width = height * Game::instance->ratio;
    inv_half_width = 2.f / width;
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

