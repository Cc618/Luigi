#pragma once

// GPU program
// Inspired by the Texture class

#include <string>
#include <unordered_map>
#include <list>
#include <GL/glew.h>

// Like Shader::create but without return
void new_shader(const std::string& name, const std::string& vertex_file,
    const std::string& fragment_file, const std::list<std::string>& uniforms);

class Shader
{
public:
    static Shader *create(const std::string& name, const std::string& vertex_file,
        const std::string& fragment_file, const std::list<std::string>& uniforms);
    // TODO : Binding
    static Shader *get(const std::string& name);

    // Create a shader named main with default vertex and fragment programs
    static void create_main();

    static Shader *create_src(const std::string& name, const std::string& vertex,
        const std::string& fragment, const std::list<std::string>& uniforms);

public:
    ~Shader();

public:
    void bind() const;

    // TODO : Python bindings
    Shader *set_uniform_1f(const std::string& name, float x);
    Shader *set_uniform_2f(const std::string& name, float x, float y);
    Shader *set_uniform_3f(const std::string& name, float x, float y, float z);
    Shader *set_uniform_4f(const std::string& name, float r, float g, float b, float a);
    // TODO : Custom matrix type
    Shader *set_uniform_mat2(const std::string& name, float a, float b, float c, float d);

public:
    // Uniform name to id
    std::unordered_map<std::string, GLint> uniforms;

private:
    static std::unordered_map<std::string, Shader*> instances;

private:
    Shader(const std::string &name, const std::string& vertex,
        const std::string& fragment, const std::list<std::string>& uniforms);

private:
    GLuint id;
    std::string name;
};
