#include "buffers.h"

constexpr size_t SQUARE_VERTEX_SIZE = 4;

using namespace lg;

// --- VBO --- //
VBO *VBO::square = nullptr;

void VBO::create_square()
{
    // x, y, 1, u, v
    GLfloat vertices[6 * 4] = {
        -.5f,   -.5f,   0,  0,
        0.5f,   -.5f,   1,  0,
        0.5f,   0.5f,   1,  1,
        0.5f,   0.5f,   1,  1,
        -.5f,   -.5f,   0,  0,
        -.5f,   0.5f,   0,  1,
    };

    square = new VBO(vertices, 6, SQUARE_VERTEX_SIZE);
}

void VBO::use_square(VBO *b)
{
    b->use();

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, b->vertex_size * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, b->vertex_size * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void VBO::unuse_square()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VBO::VBO(const GLfloat *data, size_t n_vertices, size_t vertex_size, GLenum mode)
    : n_vertices(n_vertices), vertex_size(vertex_size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, n_vertices * vertex_size * sizeof(float), data, mode);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &id);
}
