#pragma once

#include "Model.h"
#include "RenderedObject.h"

class House final : public RenderedObject
{
public:
    House(const Model *model, float xOffset, float zOffset, const Shader *shader);

    void Render() override;
};
