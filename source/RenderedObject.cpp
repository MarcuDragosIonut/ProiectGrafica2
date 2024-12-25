#include "RenderedObject.h"

RenderedObject::RenderedObject(const Shader *shader) : shader(shader)
{
    // This is called before the constructor of the derived class, so we cannot create the buffers here because we don't know the length of the vertices and normals vectors yet.
}

RenderedObject::~RenderedObject()
{
    if (!_created) return;

    glDeleteVertexArrays(1, &_vaoId);
    glDeleteBuffers(1, &_vboId);
    glDeleteBuffers(1, &_eboId);
}

void RenderedObject::Create()
{
    glGenVertexArrays(1, &_vaoId);
    glBindVertexArray(_vaoId);

    glGenBuffers(1, &_vboId);
    glBindBuffer(GL_ARRAY_BUFFER, _vboId);

    glGenBuffers(1, &_eboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboId);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (vertices.size() + normals.size()), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * vertices.size(), &vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), sizeof(glm::vec3) * normals.size(), &normals[0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<GLvoid *>(sizeof(glm::vec3) * vertices.size()));

    _created = true;
}

void RenderedObject::Render()
{
    glBindVertexArray(_vaoId);

    if (!_created) Create();
}
