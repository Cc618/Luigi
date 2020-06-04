#pragma once

// Open GL buffers (VBO)

#include "GL/glew.h"

namespace lg
{
    struct VBO
    {
    public:
        // Default VBO for sprites, a square with texture coordinates
        // A vertex is [x, y, 1, u, v] and there are 6 vertices
        static VBO *square;

    public:
        static void create_square();

        // For square like VBO (i.e. [x, y, 1, u, v] vertices)
        static void use_square(VBO *b);
        static void unuse_square();

    public:
        VBO(const GLfloat *data, size_t n_vertices, size_t vertex_size, GLenum mode=GL_STATIC_DRAW);
        ~VBO();

    public:
        // Just call draw
        // !!! The buffer must be bound and vertices attribued
        inline void draw()
        {
            glDrawArrays(GL_TRIANGLES, 0, n_vertices);
        }

        inline void use()
        {
            glBindBuffer(GL_ARRAY_BUFFER, id);
        }

    public:
        GLuint id;
        size_t vertex_size;
        size_t n_vertices;
    };
}
