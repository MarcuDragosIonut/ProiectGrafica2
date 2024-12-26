#include "Plane.h"

Plane::Plane(const Model *model, float xOffset, float yOffset, float zOffset, const Shader *shader) : RenderedObject(shader) {
    vertices = model->vertices;
    normals = model->normals;

    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i].x += xOffset;
        vertices[i].y += yOffset;
        vertices[i].z += zOffset;
    }

    colors = std::vector(vertices.size(), glm::vec3(0.8f, 0.8f, 0.8f));
}

void Plane::Render()
{
    RenderedObject::Render();

    shader->SetShadow(false);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    shader->SetShadow(true);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

}
