#pragma once
#include <glad.h>
#include <map>
#include <string>

#include "fileio.hpp"

#include "Mesh.hpp"
#include "Camera.hpp"
#include "render_data.hpp"
#include "gpu_mesh.hpp"

class Shader {
    public:
    GLuint shaderProgram;
};

class Object;
class Material;

struct MeshRenderer
{
    GLuint m_vao, m_vbo, m_ibo;
    bool vao_gen, vbo_gen, ibo_gen;
	bool useNormals;

    GLuint texture;

    GPUMesh* m_mesh;

    GLsizei m_size;

    Material* material;

	Object &object;

    MeshRenderer(Material *pMaterial, Object &objm, GPUMesh* gpumesh);
    void Render();
};

enum ShaderType {FRAGMENT = GL_FRAGMENT_SHADER, VERTEX = GL_VERTEX_SHADER};

struct CreateProgramArgument {
    std::string& path;
    ShaderType type;
};

class Renderer {
    public:
    static Camera* camera;
};