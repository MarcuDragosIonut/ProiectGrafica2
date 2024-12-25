#include "House.h"

House::House(const Model *model, const float xOffset, const float zOffset, const Shader *shader) : RenderedObject(shader)
{
    vertices = model->vertices;
    normals = model->normals;

    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i].x += xOffset;
        vertices[i].z += zOffset;
    }
}

void House::Render()
{
    RenderedObject::Render();

    glUniform1i(shader->GetUniform("codCol"), 1);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glUniform1i(shader->GetUniform("codCol"), 0);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
