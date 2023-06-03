#pragma once
#include <vector>
#include <glad.h>

#include "physics/transform.hpp"
#include "Material.hpp"

class LineRenderer
{
    public:
    Material* material;
    MaterialProperties properties;

    Transform* transform;

    std::vector<glm::vec3> lines;
    bool dynamic;
    bool dirty;

    GLuint VAO, VBO;

	LineRenderer(Material* pMaterial, Transform* trans, bool pDynamic);
	LineRenderer(Material* pMaterial, bool pDynamic = false);
    LineRenderer(bool pDynamic = false);
    
	virtual void Bind();
    virtual void Upload();
    virtual void Render();
};


