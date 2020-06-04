#include "shader.h"
#include <SFML/Graphics.hpp>
#include "error.h"
#include "io.h"
#include "game.h"

using namespace std;
using namespace lg;

// --- Internal --- //
//  Default shaders
static string main_vertex = R"(
#version 330 core

// Vertex is the local vertex location
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 tex_coords;

out vec2 tex;

// Main camera
uniform mat3 cam;
// Entity transform
uniform mat3 transform;
// Texture coordinates transform
uniform mat3 tex_transform;

void main()
{
    // Position
    vec2 point = (cam * transform * vec3(vertex, 1)).xy;
    gl_Position = vec4(point, 0, 1);

    // Texture
    tex = (tex_transform * vec3(tex_coords, 1)).xy;
}
)";

static string main_fragment = R"(
#version 330 core

out vec4 px;

in vec2 tex;

uniform sampler2D sprite;

void main()
{
    px = texture(sprite, tex);
}
)";

// Compiles a shader component (vertex / fragment)
static int genShaderComponent(const char *name, const char *source, GLenum type)
{
    int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw Error(string("Can't compile ") + name + " shader\n" + infoLog);
    }

    return shader;
}

// Generates a program on GPU and returns the handle to this program
static GLuint gen_shader(const char *vert, const char *frag,
                        const list<string> &uniforms_names, unordered_map<string, GLint> &uniforms_ids)
{
    // Compile both shaders
    int vertex = genShaderComponent("vertex", vert, GL_VERTEX_SHADER);
    int fragment = genShaderComponent("fragment", frag, GL_FRAGMENT_SHADER);

    // Link
    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    // Check errors
    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    char infoLog[512];

    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        throw Error(string("Can't link shaders\n") + infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // Setup uniforms
    for (auto name : uniforms_names)
    {
        // Find
        auto u_id = glGetUniformLocation(id, name.c_str());
        Error::check(u_id != -1, "Uniform '" + name + "' not found");

        uniforms_ids[name] = u_id;
    }

    return id;
}

// --- Static --- //
unordered_map<string, Shader *> Shader::instances;

Shader *Game::add_shader(const string &name, const string &vertex_file, const string &fragment_file, const std::list<std::string> &uniforms) const
{
    return Shader::create(name, vertex_file, fragment_file, uniforms);
}

Shader *Shader::create(const string &name, const string &vertex_file, const string &fragment_file, const std::list<std::string> &uniforms)
{
    return create_src(name, read_txt_file(vertex_file), read_txt_file(fragment_file), uniforms);
}

Shader *Shader::get(const string &name)
{
    auto result = instances.find(name);

    Error::check(result != instances.end(), "Shader with name '" + name + "' not found");

    return (*result).second;
}

void Shader::create_main()
{
    create_src("main", main_vertex, main_fragment, { "transform", "cam", "tex_transform" });
}

Shader *Shader::create_src(const string &name, const string &vertex, const string &fragment, const std::list<std::string> &uniforms)
{
    Error::check(instances.find(name) == instances.end(), "Shader with name '" + name + "' already exists");

    auto s = new Shader(name, vertex, fragment, uniforms);

    instances[name] = s;

    return s;
}

void Shader::set_main_cam(const Mat3 *mat)
{
    for (auto s : instances)
    {
        s.second->use();
        s.second->set_uniform_mat3(s.second->u_cam, mat);
    }
}

// --- Instance --- //
Shader::Shader(const string &name, const string &vert, const string &frag, const std::list<std::string> &uniforms)
    : name(name)
{
    id = gen_shader(vert.c_str(), frag.c_str(), uniforms, this->uniforms);

    // Check default uniforms
    auto cam = this->uniforms.find("cam");
    Error::check(cam != this->uniforms.end(), "The 'cam' uniform must be defined in the shader '" + name + "'");
    u_cam = (*cam).second;
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::use() const
{
    glUseProgram(id);
}

GLint Shader::get_uniform(const std::string &name) const
{
    // Find
    auto u_id = uniforms.find(name);
    Error::check(u_id != uniforms.end(), "Uniform with name '" + name + "' not found or not present in the uniform list");

    return (*u_id).second;
}

Shader *Shader::set_1f(const std::string &name, float x)
{
    return set_uniform_1f(get_uniform(name), x);
}

Shader *Shader::set_2f(const std::string &name, float x, float y)
{
    return set_uniform_2f(get_uniform(name), x, y);
}

Shader *Shader::set_3f(const std::string &name, float x, float y, float z)
{
    return set_uniform_3f(get_uniform(name), x, y, z);
}

Shader *Shader::set_4f(const std::string &name, float r, float g, float b, float a)
{
    return set_uniform_4f(get_uniform(name), r, g, b, a);
}

Shader *Shader::set_mat3(const std::string &name, const Mat3 *mat)
{
    return set_uniform_mat3(get_uniform(name), mat);
}

Shader *Shader::set_uniform_1f(GLint id, float x)
{
    glUniform1f(id, x);

    return this;
}

Shader *Shader::set_uniform_2f(GLint id, float x, float y)
{
    glUniform2f(id, x, y);

    return this;
}

Shader *Shader::set_uniform_3f(GLint id, float x, float y, float z)
{
    glUniform3f(id, x, y, z);

    return this;
}

Shader *Shader::set_uniform_4f(GLint id, float r, float g, float b, float a)
{
    glUniform4f(id, r, g, b, a);

    return this;
}

Shader *Shader::set_uniform_mat3(GLint id, const Mat3 *mat)
{
    glUniformMatrix3fv(id, 1, false, mat->data);

    return this;
}

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void bind_shader(py::module &m)
{
    py::class_<Shader>(m, "Shader")
        .def_static("get", &Shader::get, py::arg("name"),
            "Returns the shader associated to this name.")

        .def("get_uniform", &Shader::get_uniform, py::arg("name"),
            "Retrieves the id of the uniform.")

        .def("set_uniform_1f", &Shader::set_uniform_1f, py::arg("id"), py::arg("x"))
        .def("set_uniform_2f", &Shader::set_uniform_2f, py::arg("id"), py::arg("x"), py::arg("y"))
        .def("set_uniform_3f", &Shader::set_uniform_3f, py::arg("id"), py::arg("x"), py::arg("y"), py::arg("z"))
        .def("set_uniform_4f", &Shader::set_uniform_4f, py::arg("id"), py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a"))
        .def("set_uniform_mat3", &Shader::set_uniform_mat3, py::arg("id"), py::arg("mat"))
        .def("set_1f", &Shader::set_1f, py::arg("name"), py::arg("x"))
        .def("set_2f", &Shader::set_2f, py::arg("name"), py::arg("x"), py::arg("y"))
        .def("set_3f", &Shader::set_3f, py::arg("name"), py::arg("x"), py::arg("y"), py::arg("z"))
        .def("set_4f", &Shader::set_4f, py::arg("name"), py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a"))
        .def("set_mat3", &Shader::set_mat3, py::arg("name"), py::arg("mat"))

        .doc() = R"(
            (**shader**) A GPU program.

            To set uniforms, call ``set_<type>`` with the name of the uniform.
            This method is less efficient than setting a uniform by its id retrieved
            with :func:`get_uniform`, after that, call ``set_uniform_<type>``.
        )"
    ;
}
