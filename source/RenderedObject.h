#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include "Shader.h"

class RenderedObject
{
    GLuint _vaoId{}, _vboId{};

    bool _created = false;
    void Create();

protected:
    const Shader *shader;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;

public:
    explicit RenderedObject(const Shader *shader);
    virtual ~RenderedObject();

    virtual void Render();
};
