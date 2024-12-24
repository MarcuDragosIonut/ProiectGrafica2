#include "Model.h"

#include <objloader.hpp>

Model::Model(const char *file)
{
    loadOBJ(file, vertices, uvs, normals);
}

Model::~Model()
{
    vertices.clear();
    uvs.clear();
    normals.clear();
}
