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
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1}
    };
}

void Ground::Render()
{
    RenderedObject::Render();

    glUniform1i(shader->GetUniform("codCol"), 2);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
