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
    // Called each frame before draw but after update
    void update(void (*set_transform)(const Mat3 *transform));

    // Generates the transform
    Mat3 *get_transform() const;

    float get_x() const;
    float get_y() const;
    float get_width() const;
    float get_height() const;
    float get_rot() const;

    void set_height(float val);
    void set_x(float val);
    void set_y(float val);
    void set_rot(float val);

public:
    // Whether the transform has changed since the last frame
    bool transform_changed = true;

private:
    float x;
    float y;
    float width;
    float inv_width;
    float height;
    float inv_height;
    float rot;
};
