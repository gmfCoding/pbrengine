#pragma warning(disable : 4996)

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>

#include "util/ModelImporter.hpp"
#include "render_data.hpp"

Model* ModelImporter::LoadModel(const char* fileLocation)
{
	FILE* file;
	fopen_s(&file, fileLocation, "r");
	if (file == NULL) {
		printf("Cannot open the file: %s!\n", fileLocation);
		return NULL;
	}

	temp_vertices = {};
	temp_uvs = { };
	temp_normals = { };
	vertexIndices = { };
	uvIndices = { };
	normalIndices = { };
	while (1) {
		char lineHeader[128];

		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			static glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex * 0.3f);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			static glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			static std::string vertex1, vertex2, vertex3;
			static GPUIndex vertexIndex[3], uvIndex[3], normalIndex[3];
						
			int matches = fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return NULL;
			}
			else
			{
				vertexToNormal.emplace(vertexIndex[0], normalIndex[0]);
				vertexToNormal.emplace(vertexIndex[1], normalIndex[1]);
				vertexToNormal.emplace(vertexIndex[2], normalIndex[2]);

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}

		}
	}
	model = new Model;
	
	for (size_t i = 0; i < vertexIndices.size(); i++)
	{
		vertexIndices[i] -= 1;
	}
	model->vertices = temp_vertices;
	model->indices = vertexIndices;
	if (normalIndices.size() > 0)
		model->hasNormals = true;
	if (temp_normals.size() == model->indices.size())
	{
		Model flat = model->CreateFlattened();
		for (size_t i = 0; i < normalIndices.size(); i++)
		{
			flat.normals.push_back(temp_normals[normalIndices[i] - 1]);
		}
		delete model;
		return new Model(flat);
	}
	if (model->hasNormals)
	{
		for (size_t i = 0; i < model->vertices.size(); i++)
		{
			model->normals.push_back(temp_normals[this->vertexToNormal[i + 1] - 1]);
		}
	}
	return model;
}