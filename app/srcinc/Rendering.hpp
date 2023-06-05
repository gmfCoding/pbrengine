#pragma once
#include <glad.h>
#include <map>
#include <string>

#include "fileio.hpp"

#include "Mesh.hpp"
#include "Camera.hpp"

class Object;
class Material;

struct MeshRenderer
{
    GLuint m_vao, m_vbo, m_ibo;
    bool vao_gen, vbo_gen, ibo_gen;

    GLuint texture;
    Mesh* m_mesh;
    GLsizei m_size;

    Material* material;

	Object &object;

    MeshRenderer(Material *pMaterial, Object &obj);
    void Render();
    void SetVertices(Mesh* mesh, int vao, int vbo);
    void SetIndices(Mesh* mesh, int vao, int ibo, int* size, bool dynamic);

    
    void Bind(Mesh* mesh);
};

enum ShaderType {FRAGMENT = GL_FRAGMENT_SHADER, VERTEX = GL_VERTEX_SHADER};

struct CreateProgramArgument {
    std::string& path;
    ShaderType type;
};

class Shader {
    public:
    GLuint shaderProgram;
};

class Renderer {
    public:
    static Camera* camera;
};

using Index = unsigned int;

struct Vertex {
    glm::vec3  position;
    glm::vec2  texcoord;
};