#include "Mesh.hpp"
#include <vector>
#include <algorithm>
#include "render_data.hpp"

void Mesh::CameraSort(glm::vec3 cameraPos)
{
    std::vector<std::pair<float, GPUIndex>> triangleDistances;

    for (unsigned int i = 0; i < indices.size(); i += 3) {
        // Calculate the centroid of the triangle
        glm::vec3 centroid = (this->vertices[this->indices[i]] +
                            this->vertices[this->indices[i + 1]] +
                            this->vertices[this->indices[i + 2]]) / 3.0f;

        // Calculate the distance between the centroid and the camera
        float distance = glm::distance(centroid, cameraPos);

        // Add the distance and triangle index to the list
        // `1.0f / distance` causes the sort method (later on) to sort in descending order, we could instead pass `std::greater<>()` to sort.
        triangleDistances.push_back(std::make_pair(1.0f / distance, i));
    }

    // Sort the triangle distances in ascending order
    std::sort(triangleDistances.begin(), triangleDistances.end());
    
    // Reorder the triangles in the mesh based on the sorted distances
    std::vector<GPUIndex> sortedIndices;
    sortedIndices.reserve(this->indices.size());

    for (const auto& p : triangleDistances) {
        sortedIndices.push_back(this->indices[p.second]);
        sortedIndices.push_back(this->indices[p.second + 1]);
        sortedIndices.push_back(this->indices[p.second + 2]);
    }

    this->indices = sortedIndices;
}