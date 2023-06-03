#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "physics/physdata.hpp"

namespace AABBWireframe
{
    std::vector<glm::vec3> CreateWireframe(AABB* box);
}