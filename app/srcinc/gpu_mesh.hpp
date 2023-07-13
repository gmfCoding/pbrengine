#include "glad/gltypes.h"
#pragma once

class Mesh;

struct GPUMeshSpec
{
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	int ibo_size;
};

class GPUMesh
{
private:
	Mesh* mesh;

	bool dirty;
	bool dynamic;

	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_vao;
	int m_size;

	bool uvs;
	bool normals;

public:

	GPUMesh(Mesh* mesh);
	~GPUMesh();

	void RefreshIndices();
	void RefreshVertices();

	GPUMeshSpec GetSpec();
};