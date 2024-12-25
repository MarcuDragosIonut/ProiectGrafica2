#pragma once

#include "Model.h"
#include "RenderedObject.h"

class House final : public RenderedObject
{
    int _numVertices;

public:
    House(const Model *model, float xOffset, float zOffset, const Shader *shader);

    void Render() const override;
};
