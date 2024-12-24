#pragma once

#include "Shader.h"

class RenderedObject
{
    GLuint _vaoId{}, _vboId{}, _eboId{};

protected:
    const Shader *shader;

public:
    explicit RenderedObject(const Shader *shader);
    virtual ~RenderedObject();

    virtual void Render() const;
};
