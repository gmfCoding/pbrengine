#include "glad.h"
#include <map>
#include <vector>

#include "MaterialSystem.hpp"
#include "fileio.hpp"
#include "gldebug.hpp"
#include "Object.hpp"


std::map<std::string, Material*> MaterialSystem::materialMap = std::map<std::string, Material*>();

void MaterialSystem::AddMaterial(Material* material)
{
    if(materialMap.count(material->materialName) > 1)
        std::cout << "Warning: MaterialShader redefinition: " << material->materialName << std::endl;
    else
        std::cout << "MaterialShader definition: " << material->materialName << std::endl;
    materialMap[material->materialName] = material;
}

static GLuint CreateShader(const std::string& path, GLenum type)
{
    GLuint shader = glCreateShader(type);
    const std::string source = readFile(path);
    const char *shaderSrc = source.c_str();
    GLCall(glShaderSource(shader, 1, &shaderSrc, NULL));
    GLCall(glCompileShader(shader));
    MaterialSystem::DebugShaderInfo(shader);
    return shader;
}

GLuint MaterialSystem::CreateFragProgram(const std::string& fragmentPath)
{
    GLuint fragShader = CreateShader(fragmentPath, GL_FRAGMENT_SHADER);

    GLCall(GLuint program = glCreateProgram());
    GLCall(glAttachShader(program, fragShader));
    GLCall(glLinkProgram(program));
    DebugProgramInfo(program);

    GLCall(glDeleteShader(fragShader));
    return program;
}

GLuint MaterialSystem::CreateVFProgram(const std::string& vertexPath, const std::string& fragmentPath)
{
    GLuint vertShader = CreateShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragShader = CreateShader(fragmentPath, GL_FRAGMENT_SHADER);

    GLCall(GLuint program = glCreateProgram());
    GLCall(glAttachShader(program, vertShader));
    GLCall(glAttachShader(program, fragShader));
    GLCall(glLinkProgram(program));

    DebugProgramInfo(program);

    GLCall(glDeleteShader(vertShader));
    GLCall(glDeleteShader(fragShader));

    return program;
}

void MaterialSystem::DebugShaderInfo(GLuint shader)
{
    GLint result = GL_FALSE;
    int logLength;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> shaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(shader, logLength, NULL, &shaderError[0]);
}

void MaterialSystem::DebugProgramInfo(GLuint program)
{
    GLint result = GL_FALSE;
    int logLength;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError( (logLength > 1) ? logLength : 1 );
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << programError.data();
}


Material *MaterialSystem::Get(const std::string &name)
{
	return materialMap[name];
}


Material * MaterialSystem::Copy(const std::string &name)
{
	return new Material(*Get(name));
}

#include "Rendering.hpp"

void MaterialSystem::ApplyMaterial(Object &object, Material *material)
{
	material->properties.ApplyExtrinsic("MVP", Renderer::camera->preMultPV);
	material->properties.ApplyExtrinsic("FRAME", 0);

	for (auto &&i : material->properties.properties)
	{
		
	}
}