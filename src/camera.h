#pragma once

#include "maths.h"

// TODO : Binding to overload
class Camera
{
public:
    // Camera used for sprites
    static Camera *main;

public:
    Camera(float height, float x=0, float y=0, float rot=0);
    virtual ~Camera() = default;

public:
    // Generates the transform
    Mat3 *get_transform() const;

public:
    float x;
    float y;
    float width;
    float height;
    float rot;
};
