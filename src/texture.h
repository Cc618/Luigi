#pragma once

// GPU texture
// Textures are stored with create and retrieved via get static methods

#include <string>
#include <unordered_map>
#include <GL/glew.h>

namespace lg
{
    class Texture
    {
    public:
        static GLenum filter;

    public:
        static Texture *create(const std::string& name, const std::string& file);
        static Texture *get(const std::string& name);

    public:
        ~Texture();

    public:
        void use() const;

    public:
        unsigned int width;
        unsigned int height;

    private:
        static std::unordered_map<std::string, Texture*> instances;

    private:
        // Mode :
        // - pixel : No blur when scaling the texture (nearest)
        // - blur : Blur when scaling the texture (linear)
        Texture(const std::string &name, const std::string &file);

    private:
        GLuint id;
        std::string name;
    };
}
