#include "camera.h"

Camera *Camera::main = nullptr;

Mat3 *Camera::get_transform() const
{
    // TODO : Inv width and height
    return Mat3::create_transform(-x, -y, 1.f / width, 1.f / height, -rot, false);
}

Camera::Camera(float height, float x, float y, float rot)
    // TODO : Width with ratio
    : x(x), y(y), height(height), width(height), rot(rot)
{}
