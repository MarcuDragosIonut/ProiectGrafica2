#pragma once

#include "RenderedObject.h"
#include "Shader.h"

class Ground final : public RenderedObject
{
public:
    Ground(GLfloat stretch, const Shader *shader);

    void Render() override;
};
