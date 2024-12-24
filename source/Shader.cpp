#include "Shader.h"

#include <iostream>
#include <map>
#include <string>

#include "loadShaders.h"

std::map<std::string, GLint> locations{};

Shader::Shader(const char *vert, const char *frag)
{
    program = LoadShaders(vert, frag);
    glUseProgram(program);
}

Shader::~Shader()
{
    glDeleteProgram(program);
}

GLint Shader::GetUniform(const std::string &name) const
{
    if (locations.find(name) == locations.end())
    {
        locations[name] = glGetUniformLocation(program, name.c_str());
    }

    return locations[name];
}
