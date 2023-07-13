#include "util/Model.hpp"

Model::Model()
{

}

Model::Model(Model &model)
{
	this->flattened = model.flattened;
	this->hasNormals = model.hasNormals;
	this->hasUVs = model.hasUVs;

	this->vertices = model.vertices;
	this->indices = model.indices;
	this->normals = model.normals;
	this->uvs = model.uvs;

}
void Model::SetMesh(Mesh *mesh)
{
	mesh->vertices.insert(mesh->vertices.end(), this->vertices.begin(), this->vertices.end());
	mesh->indices.insert(mesh->indices.end(), this->indices.begin(), this->indices.end());
	if (this->hasNormals)
		mesh->normals.insert(mesh->normals.end(), this->normals.begin(), this->normals.end());
	if (this->hasUVs)
		mesh->uvs.insert(mesh->uvs.end(), this->uvs.begin(), this->uvs.end());
}

Model Model::CreateFlattened()
{
    std::vector<glm::vec3> temp_vertices;
	std::vector<GPUIndex> temp_indices;
    bool encountedSmall = false;
    for (unsigned int i = 0; i < indices.size(); i++) {
        if (indices[i] < 1)
        {
            encountedSmall = true;
            break;
        }
    }

    for (unsigned int i = 0; i < indices.size(); i++) {
        GPUIndex vertexIndex = indices[i];
        glm::vec3 vertex;
        if (encountedSmall) {
            vertex = vertices[vertexIndex];
        }
        else {
            vertex = vertices[vertexIndex - 1];
        }

        temp_vertices.push_back(vertex);
		temp_indices.push_back(vertexIndex);
    }

    Model flat = Model();
    flat.vertices = temp_vertices;
	flat.indices = temp_indices;
    flat.flattened = true;

    return flat;
}