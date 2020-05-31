#pragma once

// Math classes and functions

#include <GL/glew.h>

struct Mat3
{
public:
    // 2D transformation matrix
    // * translate_after describes whether we have to translate after rotation and scaling
    static Mat3 *create_transform(float x, float y, float width=1, float height=1, float rot=0, bool translate_after=true);

    // Identity
    static Mat3 *create_id();

public:
    // Matrix representation in GPU :
    // 0 3 6
    // 1 4 7
    // 2 5 8
    GLfloat data[9];
};

