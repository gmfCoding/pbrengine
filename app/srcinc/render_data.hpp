#ifndef RENDER_DATA_HPP
#define RENDER_DATA_HPP


#ifdef RENDER_DATA_ALL
#define RENDER_DATA_GL
#define RENDER_DATA_GLM
#endif

#ifdef RENDER_DATA_GL
#include "glad/gltypes.h"
#endif

#ifdef RENDER_DATA_GLM
#include "glm/fwd.hpp"

struct GPUVertex {
    glm::vec3 position;
    glm::vec2 texcoord;
	glm::vec3 normal;
};
#endif

using GPUIndex = unsigned int;
#endif