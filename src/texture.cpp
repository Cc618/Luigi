#include "texture.h"
#include <SFML/Graphics.hpp>
#include "error.h"

using namespace std;

// Generates a texture on GPU and returns the handle to this texture
// * Format is RGBA8
// TODO : filter
static GLuint genTexture(const unsigned char *data, size_t width, size_t height)
{
    GLuint id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

std::unordered_map<std::string, Texture*> Texture::instances;

void new_texture(const std::string& name, const std::string& file)
{
    Texture::create(name, file);
}

Texture *Texture::create(const std::string& name, const std::string& file)
{
    Error::check(instances.find(name) == instances.end(), "Texture with name '" + name + "' already exists");

    auto t = new Texture(name, file);

    instances[name] = t;

    return t;
}

Texture *Texture::get(const std::string& name)
{
    auto result = instances.find(name);
    
    Error::check(result != instances.end(), "Texture with name '" + name + "' not found");

    return (*result).second;
}

Texture::Texture(const std::string &name, const std::string &file)
    : name(name)
{
    // Load
    sf::Image img;
    img.loadFromFile(file);

    // Get properties
    auto size = img.getSize();
    width = size.x;
    height = size.y;

    // Send to GPU
    id = genTexture(img.getPixelsPtr(), width, height);
}

Texture::~Texture()
{
    // TMP : Destroy texture
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
    glActiveTexture(GL_TEXTURE0);
}
