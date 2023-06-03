#pragma once
#include "Material.hpp"
#include "physics/physdata.hpp"
#include "rendering/line_renderer.hpp"

class AABBRenderer : public LineRenderer
{
    public:
	AABB*		aabb;

    AABBRenderer(AABB* pAABB, Material* pMaterial, bool pDynamic = false);
    AABBRenderer(AABB* pAABB, bool pDynamic = true); // NOTE pDynamic is true by default
    
	void Upload();
};


