#ifndef MODEL_H_INCLUDE
#define MODEL_H_INCLUDE
#include <glm.hpp>
#include <vector>
#include "Mesh.hpp"
#include "render_data.hpp"

class Model
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<GPUIndex> indices;
	std::vector<glm::vec3> normals;
	std::vector<GPUIndex> normal_indices;
	std::vector<glm::vec2> uvs;

	bool flattened = false;

	bool hasUVs;
	bool hasNormals;
 	Model();
	
    Model(Model &model);

	Model CreateFlattened();
	void SetMesh(Mesh *mesh);
};
#endif