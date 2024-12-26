#pragma once

#include "Model.h"
#include "RenderedObject.h"

class Plane final: public RenderedObject{
public:
    Plane(const Model *model, float xOffset, float yOffset, float zOffset, const Shader *shader);

    void Render() override;
};
