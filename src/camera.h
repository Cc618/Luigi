#pragma once

#include "maths.h"

class Camera
{
public:
    // Camera used for sprites
    static Camera *main;

public:
    // Width is computed with the screen aspect ratio
    Camera(float height, float x=0, float y=0, float rot=0);
    virtual ~Camera() = default;

public:
    // Called each frame before draw but after update
    // set_transform is called when the transform has changed
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
    float height;
    float rot;

    // To avoid divisions
    // Halved because otherwise the viewport will have doubled size
    float inv_half_width;
    float inv_half_height;
};
