#include "Ground.h"

#include <iostream>

Ground::Ground(const GLfloat stretch, const Shader *shader) : RenderedObject(shader)
{
    vertices = {
        {-stretch, 0, -stretch},
        {stretch, 0, -stretch},
        {stretch, 0, stretch},
        {-stretch, 0, stretch}
    };

    normals = {
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    };

    colors = std::vector(vertices.size(), glm::vec3(255.f/255.f, 255.f/255.f, 255.f/255.f));
}

void Ground::Render()
{
    RenderedObject::Render();

    shader->SetShadow(false);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
