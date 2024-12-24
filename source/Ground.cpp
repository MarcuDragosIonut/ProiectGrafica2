#include "Ground.h"

#include <iostream>

Ground::Ground(const GLfloat stretch, const Shader *shader) : RenderedObject(shader)
{
    _vertexBuffer = new GLfloat[24]
    {
        -stretch, 0.f, -stretch, 0.f, 0.f, 1.f,
        stretch, 0.f, -stretch, 0.f, 0.f, 1.f,
        stretch, 0.f, stretch, 0.f, 0.f, 1.f,
        -stretch, 0.f, stretch, 0.f, 0.f, 1.f
    };

    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), _vertexBuffer, GL_STATIC_DRAW);
}

Ground::~Ground()
{
    delete[] _vertexBuffer;
}

void Ground::Render() const
{
    RenderedObject::Render();

    glUniform1i(shader->GetUniform("codCol"), 2);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
