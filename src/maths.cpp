#include "maths.h"
#include <cmath>

float pi = 3.14159265f;

// --- Mat3 --- //
Mat3 *Mat3::create_tsr(float x, float y, float width, float height, float rot)
{
    Mat3 *m = new Mat3();

    GLfloat i = sinf(rot);
    GLfloat j = cosf(rot);

    m->data[0] = width * j; m->data[3] = -height * i;   m->data[6] = m->data[0] * x + m->data[3] * y;
    m->data[1] = width * i; m->data[4] = height * j;    m->data[7] = m->data[1] * x + m->data[4] * y;
    m->data[2] = 0;         m->data[5] = 0;             m->data[8] = 1;

    return m;
}

Mat3 *Mat3::create_rst(float x, float y, float width, float height, float rot)
{
    Mat3 *m = new Mat3();

    GLfloat i = sinf(rot);
    GLfloat j = cosf(rot);

    m->data[0] = width * j;     m->data[3] = -width * i;    m->data[6] = x;
    m->data[1] = height * i;    m->data[4] = height * j;    m->data[7] = y;
    m->data[2] = 0;             m->data[5] = 0;             m->data[8] = 1;

    return m;
}

Mat3 *Mat3::create_srt(float x, float y, float width, float height, float rot)
{
    Mat3 *m = new Mat3();

    GLfloat i = sinf(rot);
    GLfloat j = cosf(rot);

    m->data[0] = width * j;     m->data[3] = height * i;    m->data[6] = x;
    m->data[1] = -width * i;    m->data[4] = height * j;    m->data[7] = y;
    m->data[2] = 0;             m->data[5] = 0;             m->data[8] = 1;

    return m;
}

Mat3 *Mat3::create_trs(float x, float y, float width, float height, float rot)
{
    Mat3 *m = new Mat3();

    GLfloat i = sinf(rot);
    GLfloat j = cosf(rot);

    m->data[0] = width * j;     m->data[3] = width * i;     m->data[6] = m->data[0] * x + m->data[3] * y;
    m->data[1] = -height * i;   m->data[4] = height * j;    m->data[7] = m->data[1] * x + m->data[4] * y;
    m->data[2] = 0;             m->data[5] = 0;             m->data[8] = 1;

    return m;
}

Mat3 *Mat3::create_id()
{
    Mat3 *m = new Mat3();

    m->data[0] = 1;
    m->data[1] = 0;
    m->data[2] = 0;
    m->data[3] = 0;
    m->data[4] = 1;
    m->data[5] = 0;
    m->data[6] = 0;
    m->data[7] = 0;
    m->data[8] = 1;

    return m;
}

// --- Box --- //
Box::Box(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
    : x(x), y(y), width(width), height(height)
{}

Mat3 *Box::get_transform() const
{
    auto m = new Mat3();

    m->data[0] = width; m->data[3] = 0;         m->data[6] = x;
    m->data[1] = 0;     m->data[4] = height;    m->data[7] = y;
    m->data[2] = 0;     m->data[5] = 0;         m->data[8] = 1;

    return m;
}

bool Box::contains(float _x, float _y) const
{
    return _x >= x && _x <= x + width && _y >= y && _y <= y + height;
}

std::string Box::__repr__() const
{
    return std::string("Box(x=") + std::to_string(x) + ", y=" + std::to_string(y) +
        ", width=" + std::to_string(width) + ", height=" + std::to_string(height) + ")";
}
