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

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
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
