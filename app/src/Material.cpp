#include "Material.hpp"
#include "MaterialSystem.hpp"
#include <map>
#include <any>
#include "glad.h"
#include <glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#define GLCase(type, rtype, func) case type: \
			func(this->location, std::any_cast<rtype>(gl_case_value)); \
			break

#define GLCaseV(TYPE, rtype, FUNC) case TYPE: \
			FUNC(this->location, 1, glm::value_ptr(std::any_cast<rtype>(gl_case_value))); \
			break

void Material::Setup() {

}

void Material::Bind() {
    glUseProgram(this->programID);
}


Material::Material(const std::string _materialName) : materialName(_materialName)
{
    MaterialSystem::AddMaterial(this);
}

Material::Material(const Material &source)
{
	this->programID = source.programID;
	this->properties = source.properties;
	this->materialName = source.materialName;
}

VertexFragmentCombinationMaterial::VertexFragmentCombinationMaterial(const std::string _materialName, std::string _vertexPath, std::string _fragmentPath) : Material(_materialName), vertexPath(_vertexPath), fragmentPath(_fragmentPath)
{
    programID = MaterialSystem::CreateVFProgram(vertexPath, fragmentPath);
}


bool MaterialProperties::HasExtrinsic(const std::string& name)
{
    std::map<std::string, Matprop>::iterator it = this->extrinsic.find(name);
    return it != this->extrinsic.end();
}

bool MaterialProperties::HasValue(const std::string& name)
{
    std::map<std::string, Matprop>::iterator it = this->properties.find(name);
    return it != this->properties.end();
}

void Matprop::Apply()
{
	ApplyOverride(this->value);
}

void Matprop::ApplyOverride(std::any gl_case_value)
{
	if (this->location <= -1)
		return;
	switch (type)
	{
		GLCase(BOOL, GLboolean, glUniform1ui);

		GLCase(INT1, GLint, glUniform1i);
		GLCaseV(INT2, glm::ivec2, glUniform2iv);
		GLCaseV(INT3, glm::ivec3, glUniform3iv);
		GLCaseV(INT4, glm::ivec4, glUniform4iv);

		GLCase(UINT1, GLuint, glUniform1ui);
		GLCaseV(UINT2, glm::uvec2, glUniform2uiv);
		GLCaseV(UINT3, glm::uvec3, glUniform3uiv);
		GLCaseV(UINT4, glm::uvec4, glUniform4uiv);

		GLCase(FLOAT1, GLfloat, glUniform1f);
		GLCaseV(FLOAT2, glm::fvec2, glUniform2fv);
		GLCaseV(FLOAT3, glm::fvec3, glUniform3fv);
		GLCaseV(FLOAT4, glm::fvec4, glUniform4fv);
		case MAT4:
			glUniformMatrix4fv(this->location, 1, GL_FALSE, glm::value_ptr(std::any_cast<glm::mat4>(gl_case_value)));
			break;
	default:
		break;
	}
}