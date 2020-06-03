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
bool Box::collides(const Box& other) const
{
    return (x >= other.x && x <= other.x + other.width || other.x >= x && other.x <= x + width) &&
        (y >= other.y && y <= other.y + other.height || other.y >= y && other.y <= y + height);
}

std::string Box::__repr__() const
{
    return std::string("Box(x=") + std::to_string(x) + ", y=" + std::to_string(y) +
        ", width=" + std::to_string(width) + ", height=" + std::to_string(height) + ")";
}

// --- Bindings --- //
#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_maths(py::module &m)
{
    m.attr("pi") = pi;

    py::class_<Mat3>(m, "Mat3")
        .def_static("create_tsr", &Mat3::create_tsr, py::arg("x"), py::arg("y"), py::arg("width")=1, py::arg("height")=1, py::arg("rot")=0,
            "Translate -> Scale -> Rotate transformation matrix.")

        .def_static("create_rst", &Mat3::create_rst, py::arg("x"), py::arg("y"), py::arg("width")=1, py::arg("height")=1, py::arg("rot")=0,
            "Rotate -> Scale -> Translate transformation matrix.")

        .def_static("create_srt", &Mat3::create_srt, py::arg("x"), py::arg("y"), py::arg("width")=1, py::arg("height")=1, py::arg("rot")=0,
            "Scale -> Rotate -> Translate transformation matrix.")

        .def_static("create_trs", &Mat3::create_trs, py::arg("x"), py::arg("y"), py::arg("width")=1, py::arg("height")=1, py::arg("rot")=0,
            "Translate -> Rotate -> Scale transformation matrix.")

        .def_static("create_id", &Mat3::create_id,
            "Identity matrix.")

        .doc() = "(**maths**) A 3x3 matrix."
    ;

    py::class_<Box>(m, "Box")
        .def(py::init<GLfloat, GLfloat, GLfloat, GLfloat>(), py::arg("x")=0, py::arg("y")=0, py::arg("width")=0, py::arg("height")=0)

        .def_readwrite("x", &Box::x)
        .def_readwrite("y", &Box::y)
        .def_readwrite("width", &Box::width)
        .def_readwrite("height", &Box::height)

        .def("contains", &Box::contains, py::arg("x"), py::arg("y"),
            "Whether the box contains the point.")

        .def("collides", &Box::collides, py::arg("other"),
            "Whether the box collides with another box.")

        .def("__repr__", &Box::__repr__)

        .doc() = "(**maths**) A 2D region describing an axis aligned bounding box."
    ;
}
