#include "error.h"

using namespace std;
using namespace lg;

Error::Error(const std::string& msg)
    : msg("(Luigi) " + msg)
{}

const char *Error::what() const noexcept
{
    return msg.c_str();
}

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void bind_error(py::module &m)
{
    py::register_exception<Error>(m, "Error").doc() =
        "(**error**) A Luigi exception, use it only from C++.";
}
