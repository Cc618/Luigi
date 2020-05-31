#include "maths.h"
#include <cmath>

// --- Mat3 --- //
// Rotate -> Scale -> Translate
// Rotate * Scale = [a, b, c, d] as a Mat2
Mat3 *Mat3::create_transform(float x, float y, float width, float height, float rot, bool translate_after)
{
    Mat3 *m = new Mat3();

    GLfloat i = cosf(rot);
    GLfloat j = sinf(rot);

    GLfloat a = width * i;
    GLfloat b = -height * j;
    GLfloat c = width * j;
    GLfloat d = height * i;

    // Rotate * Scale
    m->data[0] = a; m->data[3] = b;
    m->data[1] = c; m->data[4] = d;

    // Translate
    if (translate_after)
    {
        m->data[6] = x;
        m->data[7] = y;
    }
    else
    {
        m->data[6] = x * a + y * b;
        m->data[7] = x * c + y * d;
    }

    // Last row
    m->data[2] = 0;
    m->data[5] = 0;
    m->data[8] = 1;

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
