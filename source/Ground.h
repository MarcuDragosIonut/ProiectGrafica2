#pragma once

#include "RenderedObject.h"
#include "Shader.h"

class Ground final : public RenderedObject
{
    GLfloat *_vertexBuffer;

public:
    Ground(GLfloat stretch, const Shader *shader);
    ~Ground() override;

    void Render() const override;
};
