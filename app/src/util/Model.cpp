#include "util/Model.hpp"


void Model::SetMesh(Mesh *mesh)
{
	mesh->vertices.insert(mesh->vertices.end(), this->vertices.begin(), this->vertices.end());
	mesh->indices.insert(mesh->indices.end(), this->indices.begin(), this->indices.end());
}

Model Model::CreateFlattened()
{
    std::vector<glm::vec3> temp_vertices;
    bool encountedSmall = false;
    for (unsigned int i = 0; i < indices.size(); i++) {
        if (indices[i] < 1)
        {
            encountedSmall = true;
            break;
        }
    }

    for (unsigned int i = 0; i < indices.size(); i++) {
        unsigned int vertexIndex = indices[i];
        glm::vec3 vertex;
        if (encountedSmall)
        {
            vertex = vertices[vertexIndex];
        }
        else {
            vertex = vertices[vertexIndex - 1];
        }

        temp_vertices.push_back(vertex);
    }

    Model flat = Model();
    flat.vertices = temp_vertices;
    flat.flattened = true;

    return flat;
}