#pragma once

// Math classes and functions

#include <GL/glew.h>

struct Mat3
{
public:
    // 2D transformation matrix
    static Mat3 *create_transform(float x, float y, float width, float height, float rot);

    // Identity
    static Mat3 *create_id();

public:
    // Matrix representation in GPU :
    // 0 3 6
    // 1 4 7
    // 2 5 8
    GLfloat data[9];
};

