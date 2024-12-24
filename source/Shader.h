#pragma once

#include <string>
#include <GL/glew.h>

struct Shader
{
    GLuint program;

    Shader(const char *vert, const char *frag);
    ~Shader();

    [[nodiscard]] GLint GetUniform(const std::string &name) const;
};
