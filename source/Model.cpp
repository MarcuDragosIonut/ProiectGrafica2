#include "Model.h"

#include <objloader.hpp>
#include <map>
#include <iostream>
#include <glm/geometric.hpp>

Model::Model(const char *file)
{
    loadOBJ(file, vertices, uvs, normals);
    //averageNormals();
}

Model::~Model()
{
    vertices.clear();
    uvs.clear();
    normals.clear();
}

struct Vec3Comparator{
    bool operator()(const glm::vec3& v1, const glm::vec3& v2) const{
        if(v1.x != v2.x) return v1.x < v2.x;
        if(v1.y != v2.y) return v1.y < v2.y;
        return v1.z < v2.z;
    }
};

void Model::averageNormals() {
    std::map<glm::vec3, std::vector<int>, Vec3Comparator> vertexIndexMap;
    std::map<glm::vec3, glm::vec3, Vec3Comparator> individualNormals;
    for(int i = 0; i < vertices.size(); i++){
        glm::vec3 vertex(vertices[i][0], vertices[i][1], vertices[i][2]);
        vertexIndexMap[vertex].push_back(i);
        if(!individualNormals.count(vertex)) individualNormals[vertex] = glm::vec3(0,0,0);
        individualNormals[vertex] += normals[i];
    }
    for(auto &v: vertexIndexMap){
        //std::cout << v.first.x << ' ' << v.first.y << ' ' << v.first.z << " : ";
        for(auto &vertexIndex: v.second){
            normals[vertexIndex] = glm::normalize(individualNormals[v.first]);
            //std::cout << individualNormals[v.first].x << ' ' << individualNormals[v.first].y << ' ' << individualNormals[v.first].z << '\n';
        }
        //std::cout << "norm: " << normals[v.second[0]].x << ' ' << normals[v.second[0]].y << ' ' << normals[v.second[0]].z << '\n';
    }
}