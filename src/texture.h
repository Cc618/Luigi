#pragma once

// GPU texture
// Textures are stored with create and retrieved via get static methods

#include <string>
#include <unordered_map>
#include <GL/glew.h>

// Like Texture::create but without return
void new_texture(const std::string& name, const std::string& file);

class Texture
{
public:
    static Texture *create(const std::string& name, const std::string& file);
    static Texture *get(const std::string& name);

public:
    ~Texture();

public:
    void bind() const;

public:
    unsigned int width;
    unsigned int height;

private:
    static std::unordered_map<std::string, Texture*> instances;

private:
    // TODO : Mode (nearest...)
    Texture(const std::string &name, const std::string &file);

private:
    GLuint id;
    std::string name;
};
