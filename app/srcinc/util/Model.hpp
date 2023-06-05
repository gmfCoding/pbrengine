#ifndef MODEL_H_INCLUDE
#define MODEL_H_INCLUDE
#include <glm.hpp>
#include <vector>
#include "Mesh.hpp"

class Model
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> normal_indices;
	
	bool flattened = false;

	Model CreateFlattened();
	void SetMesh(Mesh *mesh);
};
#endif