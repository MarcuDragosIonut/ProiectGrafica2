#include "RenderedObject.h"

RenderedObject::RenderedObject(const Shader *shader) : shader(shader)
{
    glGenVertexArrays(1, &_vaoId);
    glBindVertexArray(_vaoId);

    glGenBuffers(1, &_vboId);
    glBindBuffer(GL_ARRAY_BUFFER, _vboId);

    glGenBuffers(1, &_eboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboId);
}

RenderedObject::~RenderedObject()
{
    glDeleteVertexArrays(1, &_vaoId);
    glDeleteBuffers(1, &_vboId);
    glDeleteBuffers(1, &_eboId);
}

void RenderedObject::Render() const
{
    glBindVertexArray(_vaoId);
}
