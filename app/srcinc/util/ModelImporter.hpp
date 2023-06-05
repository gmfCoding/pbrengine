#pragma warning(disable : 4996)
#ifndef MODEL_IMPORTER_H
#define MODEL_IMPORTER_H
#include <vector>
#include <glm.hpp>
#include "Model.hpp"

class ModelImporter
{
public:

	std::vector<unsigned short> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	Model* model = nullptr;

	Model* LoadModel(const char* fileLocation);
};
#endif