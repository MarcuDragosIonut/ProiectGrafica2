﻿#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Model
{
public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    explicit Model(const char *file);
    ~Model();
private:
    void averageNormals();
};
