#pragma once

// GPU program
// Inspired by the Texture class

#include <string>
#include <unordered_map>
#include <list>
#include <GL/glew.h>
#include "maths.h"

// Like Shader::create but without return
// TODO : Rename to add_shader + mv to game + returns Shader
void new_shader(const std::string& name, const std::string& vertex_file,
    const std::string& fragment_file, const std::list<std::string>& uniforms);

namespace lg
{
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

        // Sets the main camera for each shader (as a matrix)
        static void set_main_cam(const Mat3 *mat);

    public:
        ~Shader();

    public:
        void use() const;

        // Retrieves the id of a uniform
        GLint get_uniform(const std::string& name) const;

        // Set uniforms
        // !!! These functions must be called in the draw method when the shader is bound (with use)
        // * Note that 'this' is returned to allow chains
        // To optimize, use set_uniform with the id retrieved by get_uniform
        Shader *set_1f(const std::string& name, float x);
        Shader *set_2f(const std::string& name, float x, float y);
        Shader *set_3f(const std::string& name, float x, float y, float z);
        Shader *set_4f(const std::string& name, float r, float g, float b, float a);
        Shader *set_mat3(const std::string& name, const Mat3 *mat);

        // Faster calls with ids retrieved with get_uniform
        Shader *set_uniform_1f(GLint id, float x);
        Shader *set_uniform_2f(GLint id, float x, float y);
        Shader *set_uniform_3f(GLint id, float x, float y, float z);
        Shader *set_uniform_4f(GLint id, float r, float g, float b, float a);
        Shader *set_uniform_mat3(GLint id, const Mat3 *mat);

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

        // The cam uniform is a mat3x3 that describes the main camera used for sprites
        GLint u_cam;
    };
}
