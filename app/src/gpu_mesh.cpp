
#include <glm.hpp>
#include <vector>
#include <glad/glad.h>

#define RENDER_DATA_GLM
#include "render_data.hpp"
#include "gpu_mesh.hpp"
#include "Mesh.hpp"
#include "gldebug.hpp"

GPUMesh::GPUMesh(Mesh* p_mesh) : mesh(p_mesh), m_vao(0), m_vbo(0), m_ibo(0), m_size(0), dynamic(false)
{

	this->normals = mesh->normals.size() >= 1 && mesh->normals.size() == mesh->vertices.size();
	this->uvs = mesh->uvs.size() >= 1 && mesh->uvs.size() == mesh->vertices.size();
	GLCall(glGenVertexArrays(1, &m_vao)); // Vertex  Array  Object

	GLCall(glGenBuffers(1, &m_vbo)); // Vertex  Buffer Object (temp)
	GLCall(glGenBuffers(1, &m_ibo)); // Element Buffer Object (temp)

    GLCall(glBindVertexArray(m_vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

    RefreshVertices();
    RefreshIndices();



    GLCall(glBindVertexArray(0));
}

GPUMesh::~GPUMesh()
{

}

GPUMeshSpec GPUMesh::GetSpec()
{
	return (GPUMeshSpec){m_vao, m_vbo, m_ibo, m_size};
}

void GPUMesh::RefreshVertices()
{ 
    GLCall(glBindVertexArray(m_vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    
	std::vector<GPUVertex> vertices_uv;
	for (size_t i = 0; i < mesh->vertices.size(); i++)
	{
		GPUVertex vertex;
		vertex.position = mesh->vertices[i];
		if(uvs)
			vertex.texcoord = mesh->uvs[i];
		if(normals)
			vertex.normal = mesh->normals[i];
		vertices_uv.push_back(vertex);
	}
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices_uv.size() * sizeof(GPUVertex), vertices_uv.data(), GL_STATIC_DRAW)); 
	// Location of position
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), 0));
	// Location of Texcoords
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), reinterpret_cast<const void*>(offsetof(GPUVertex, texcoord))));
	// Location of normals
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), reinterpret_cast<const void*>(offsetof(GPUVertex, normal))));
}

void GPUMesh::RefreshIndices()
{
    GLCall(glBindVertexArray(m_vao));

	m_size = mesh->indices.size();

    // NOTE: Do we need a currently bound vao to buffer/bind the ibo?
    // Fill the currently bound GL_ELEMENT_ARRAY_BUFFER buffer (ibo) with the data in indices
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size * sizeof(GPUIndex), mesh->indices.data(), dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}
