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

    colors = std::vector(vertices.size(), glm::vec3(0.59f, 0.29f, 0.0f));
}

void House::Render()
{
    RenderedObject::Render();

    shader->SetShadow(false);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());



    shader->SetShadow(true);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

}
