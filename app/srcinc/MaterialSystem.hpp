#pragma once
#include <map>
#include <string>
#include <GLFW/glfw3.h>

#include "Material.hpp"


class Object;

class MaterialSystem {
    public:

    static std::map<std::string, Material*> materialMap;

	static Material *Get(const std::string &name);
	static Material *Copy(const std::string &name);

    static void AddMaterial(Material* material);

    static GLuint CreateFragProgram(const std::string&  fragmentPath);
    static GLuint CreateVFProgram(const std::string& vertexPath, const std::string&  fragmentPath);
    static void DebugShaderInfo(GLuint shader);
    static void DebugProgramInfo(GLuint program);

	static void ApplyMaterial(Object &object, Material *material);
};