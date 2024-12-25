#pragma once

#include <string>
#include <GL/glew.h>

class Shader
{
    GLuint _program;

public:
    [[nodiscard]] GLint GetUniform(const std::string &name) const; // TODO: should be private

    Shader(const char *vert, const char *frag);
    ~Shader();

    void Reset() const;
    void SetShadow(bool shadow) const;
};
