#pragma once

// Math classes and functions

#include <string>
#include <GL/glew.h>

extern float pi;

namespace lg
{
    struct Mat3
    {
    public:
        // 2D transformation matrices
        // Translate -> Scale -> Rotate
        static Mat3 *create_tsr(float x, float y, float width=1, float height=1, float rot=0);

        // Rotate -> Scale -> Translate
        static Mat3 *create_rst(float x, float y, float width=1, float height=1, float rot=0);

        // Scale -> Rotate -> Translate
        static Mat3 *create_srt(float x, float y, float width=1, float height=1, float rot=0);

        // Translate -> Rotate -> Scale
        static Mat3 *create_trs(float x, float y, float width=1, float height=1, float rot=0);

        // Identity
        static Mat3 *create_id();

    public:
        // Matrix representation in GPU :
        // 0 3 6
        // 1 4 7
        // 2 5 8
        GLfloat data[9];
    };

    struct Box
    {
    public:
        Box(GLfloat x=0, GLfloat y=0, GLfloat width=0, GLfloat height=0);

    public:
        // Returns a transform describing the box
        // Useful to get texture transformation matrix
        Mat3 *get_transform() const;

        // Whether the point is inside the box
        bool contains(float x, float y) const;

        // AABB - AABB collision
        bool collides(const Box& other) const;

    public:
        std::string __repr__() const;

    public:
        GLfloat x, y, width, height;
    };
}
