#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "render_data.hpp"

class Mesh {
    public:
    std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
    std::vector<GPUIndex> indices;
    std::vector<glm::vec2> uvs;
	
    void CameraSort(glm::vec3 camera);
};