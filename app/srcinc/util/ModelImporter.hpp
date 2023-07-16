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
	Model* model = nullptr;

	Model* LoadModel(const char* fileLocation);
};
#endif