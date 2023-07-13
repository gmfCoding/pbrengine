#pragma warning(disable : 4996)
#ifndef MODEL_IMPORTER_H
#define MODEL_IMPORTER_H
#include <vector>
#include <glm.hpp>
#include "Model.hpp"
#include "render_data.hpp"
#include <unordered_map>

class ModelImporter
{
public:

	std::vector<GPUIndex> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::unordered_map<GPUIndex, GPUIndex> vertexToNormal;
	
	Model* model = nullptr;

	Model* LoadModel(const char* fileLocation);
};
#endif