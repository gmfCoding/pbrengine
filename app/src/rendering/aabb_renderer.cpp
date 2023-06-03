#include <glm/vec3.hpp>
#include <vector>
#include <glad.h>

#include "rendering/aabb_renderer.hpp"
#include "MaterialSystem.hpp"
#include "Material.hpp"
#include "typeutil/AABBWireframe.hpp"

AABBRenderer::AABBRenderer(AABB* pAABB, Material* pMaterial, bool pDynamic) : LineRenderer::LineRenderer(pMaterial, pDynamic), aabb(pAABB)
{

}


AABBRenderer::AABBRenderer(AABB* pAABB, bool pDynamic) : AABBRenderer::AABBRenderer(pAABB, MaterialSystem::materialMap["basic"], pDynamic)
{

}

void AABBRenderer::Upload()
{
	lines = AABBWireframe::CreateWireframe(aabb);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lines.size(), lines.data(), dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}