#include "Shader.h"

#include <iostream>
#include <map>
#include <string>

#include "loadShaders.h"

std::map<std::string, GLint> locations{};

Shader::Shader(const char *vert, const char *frag)
{
    _program = LoadShaders(vert, frag);
    glUseProgram(_program);
}

Shader::~Shader()
{
    glDeleteProgram(_program);
}

GLint Shader::GetUniform(const std::string &name) const
{
    if (locations.find(name) == locations.end())
    {
        locations[name] = glGetUniformLocation(_program, name.c_str());
    }

    return locations[name];
}

void Shader::Reset() const
{
    SetShadow(false);
}

void Shader::SetShadow(const bool shadow) const
{
    glUniform1i(GetUniform("codCol"), !shadow);
}
