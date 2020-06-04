#include "texture.h"
#include <SFML/Graphics.hpp>
#include "error.h"

using namespace std;

// Generates a texture on GPU and returns the handle to this texture
// * Format is RGBA8
static GLuint genTexture(const unsigned char *data, size_t width, size_t height, GLenum usage)
{
    GLuint id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, usage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, usage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

std::unordered_map<std::string, Texture*> Texture::instances;

void new_texture(const std::string& name, const std::string& file, const std::string& mode)
{
    Texture::create(name, file, mode);
}

Texture *Texture::create(const std::string& name, const std::string& file, const std::string& mode)
{
    Error::check(instances.find(name) == instances.end(), "Texture with name '" + name + "' already exists");

    auto t = new Texture(name, file, mode);

    instances[name] = t;

    return t;
}

Texture *Texture::get(const std::string& name)
{
    auto result = instances.find(name);
    
    Error::check(result != instances.end(), "Texture with name '" + name + "' not found");

    return (*result).second;
}

Texture::Texture(const std::string &name, const std::string &file, const std::string &mode)
    : name(name)
{
    // Load
    sf::Image img;
    Error::check(img.loadFromFile(file), "Can't load texture '" + name + "' from file '" + file + "'");

    // Get properties
    auto size = img.getSize();
    width = size.x;
    height = size.y;

    GLenum usage;
    if (mode == "pixel")
        usage = GL_NEAREST;
    else if (mode == "blur")
        usage = GL_LINEAR;
    else
        throw Error("Mode '" + mode + "' invalid, use either 'pixel' or 'blur'");

    // Send to GPU
    id = genTexture(img.getPixelsPtr(), width, height, usage);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::use() const
{
    glBindTexture(GL_TEXTURE_2D, id);
    glActiveTexture(GL_TEXTURE0);
}

// --- Bindings --- //
#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_texture(py::module &m)
{
    // TODO : Add texture class

    m.def("new_texture", &new_texture, py::arg("name"), py::arg("file"), py::arg("mode")="pixel",
        py::doc(R"(
            (**texture**) Creates a new texture.
            
            :param mode: The filter applied to the texture.

            **Filter types** :

            * pixel : No blur when scaling the texture (nearest).
            * blur : Blur when scaling the texture (linear).
        )"));
}
