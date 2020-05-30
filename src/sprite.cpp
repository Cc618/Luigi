#include "sprite.h"
#include <iostream>
#include <cmath>

using namespace std;

constexpr size_t SPRITE_VERTEX_SIZE = 4;

// TODO : In files
const char *sprite_vertex = R"(
#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;

uniform vec2 pos;
uniform mat2 transform;

void main()
{
    vec2 point = transform * vertex + pos;
    gl_Position = vec4(point, 0, 1);

    texCoords = inTexCoords;
}
)";

const char *sprite_fragment = R"(
#version 330 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D sprite;

void main()
{
    FragColor = texture(sprite, texCoords);
}
)";


void genTransform(float *data, float rot, float width, float height)
{
    data[0] = width * cosf(rot);
    data[1] = width * sinf(rot);
    data[2] = -sinf(rot) * height;
    data[3] = height * cosf(rot);
}

// Compiles a shader component (vertex / fragment)
int genShaderComponent(const char *name, const char *source, GLenum type)
{
    int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        // TODO : Errors
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cerr << "Error : (" << name << ") Can't compile shader\n"
             << infoLog << endl;
    }

    return shader;
}

// Ids
GLuint Sprite::vbo_id;
int Sprite::shader_id;

// Uniform locations
int Sprite::u_pos;
int Sprite::u_transform;

void Sprite::init_sprites()
{
    // Generate VBO //
    // x, y, u, v
    float vertices[6 * SPRITE_VERTEX_SIZE] = {
        -.5f,   -.5f,   0.f,  0.f,
        0.5f,   -.5f,   1.f,  0.f,
        0.5f,   0.5f,   1.f,  1.f,
        0.5f,   0.5f,   1.f,  1.f,
        -.5f,   -.5f,   0.f,  0.f,
        -.5f,   0.5f,   0.f,  1.f,
    };

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, 6 * SPRITE_VERTEX_SIZE * sizeof(float), vertices, GL_STATIC_DRAW);

    // Generate shader //
    // Compile both shaders
    int vertex = genShaderComponent("vertex", sprite_vertex, GL_VERTEX_SHADER);
    int fragment = genShaderComponent("fragment", sprite_fragment, GL_FRAGMENT_SHADER);

    // Link
    shader_id = glCreateProgram();
    glAttachShader(shader_id, vertex);
    glAttachShader(shader_id, fragment);
    glLinkProgram(shader_id);
 
    // Check errors
    int success;
    char infoLog[512];
    glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
 
    if (!success)
    {
        // TODO : Errors
        glGetProgramInfoLog(shader_id, 512, NULL, infoLog);
        cout << "Error : Can't link shaders :\n" << infoLog << endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // Setup uniforms
    u_transform = glGetUniformLocation(shader_id, "transform");
    u_pos = glGetUniformLocation(shader_id, "pos");

    if (u_transform == -1)
    {
        // TODO : Error 
        cout << "Error : Can't find transform uniform\n";
    }

    if (u_pos == -1)
    {
        // TODO : Error 
        cout << "Error : Can't find pos uniform\n";
    }
}

Sprite::Sprite(const string& texture)
    : texture(Texture::get(texture))
{
    width = this->texture->width;
    height = this->texture->height;
}

void Sprite::draw()
{
    // Send uniforms to the shader
    set_uniforms();

    // Set shader
    glUseProgram(shader_id);

    // Set Texture
    texture->bind();

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void Sprite::set_uniforms() const
{
    glUseProgram(shader_id);

    // TODO : One transformation matrix

    // Position
    glUniform2f(u_pos, x, y);

    // Transform
    // TODO : See flip y for cam or for sprite
    float transform[4];
    genTransform(transform, rot, width, -height);
    glUniformMatrix2fv(u_transform, 1, false, transform);
}
