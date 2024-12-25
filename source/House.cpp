#include "House.h"

House::House(const Model *model, const float xOffset, const float zOffset, const Shader *shader) : RenderedObject(shader)
{
    _numVertices = model->vertices.size();

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (model->vertices.size() + model->normals.size()), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * model->vertices.size(), &model->vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * model->vertices.size(), sizeof(glm::vec3) * model->normals.size(), &model->normals[0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(sizeof(glm::vec3) * model->vertices.size()));
}

void House::Render() const
{
    RenderedObject::Render();

    glUniform1i(shader->GetUniform("codCol"), 1);
    glDrawArrays(GL_TRIANGLES, 0, _numVertices);

    glUniform1i(shader->GetUniform("codCol"), 0);
    glDrawArrays(GL_TRIANGLES, 0, _numVertices);
}
