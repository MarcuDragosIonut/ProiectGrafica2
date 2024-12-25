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
}

void RenderedObject::Create()
{
    while (colors.size() < vertices.size()) colors.push_back(glm::vec3(0, 0, 0));
    while (normals.size() < vertices.size()) normals.push_back(glm::vec3(0, 0, 0));

    if (colors.size() > vertices.size()) colors.resize(vertices.size());
    if (normals.size() > vertices.size()) normals.resize(vertices.size());

    glGenVertexArrays(1, &_vaoId);
    glBindVertexArray(_vaoId);

    glGenBuffers(1, &_vboId);
    glBindBuffer(GL_ARRAY_BUFFER, _vboId);

    const int subDataSize = sizeof(glm::vec3) * vertices.size();

    glBufferData(GL_ARRAY_BUFFER, subDataSize * 3, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, subDataSize * 0, subDataSize, &vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, subDataSize * 1, subDataSize, &colors[0]);
    glBufferSubData(GL_ARRAY_BUFFER, subDataSize * 2, subDataSize, &normals[0]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<GLvoid *>(subDataSize * 0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<GLvoid *>(subDataSize * 1));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<GLvoid *>(subDataSize * 2));

    _created = true;
}

void RenderedObject::Render()
{
    glBindVertexArray(_vaoId);

    if (!_created) Create();
}
