cmake_minimum_required(VERSION 3.10)

# Create bin directory
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# To avoid opengl warnings
set(OpenGL_GL_PREFERENCE GLVND)

# Source files
file(GLOB SRC "*.cpp")

add_library(luigi MODULE ${SRC})

# C++ standard (14)
set_target_properties(luigi
    PROPERTIES
    CXX_STANDARD 14
)

add_subdirectory(pybind11)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(luigi
    pybind11::module
    sfml-graphics sfml-window sfml-system
    GL GLEW
)

# Rename the library
set_target_properties(luigi PROPERTIES PREFIX "${PROJECT_SOURCE_DIR}/bin/${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}")
