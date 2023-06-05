#pragma once

#include <string>
#include <map>
#include <any>

#include "glad.h"


struct Matprop
{
	enum PropType
	{
		BOOL,
		INT1,
		INT2,
		INT3,
		INT4,
		UINT1,
		UINT2,
		UINT3,
		UINT4,
		FLOAT1,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		MAT4
	};

	std::any value;
	PropType type;
	GLint location;

	template<typename T>
    T GetValue()
    {
        return std::any_cast<T>(value);
    }

    template<typename T>
    T& GetValueRef()
    {
        return std::any_cast<T&>(value);
    }

	void Apply();
	void ApplyOverride(std::any value);
};

struct MatpropInit
{
	std::string name;
	Matprop::PropType type;
	std::any value;
};

class MaterialProperties
{
    public:
    std::map<std::string, Matprop> properties;
	std::map<std::string, Matprop> extrinsic;

    public:
    bool HasValue(const std::string& name);
    bool HasExtrinsic(const std::string& name);

    template<typename T>
    bool GetValue(const std::string& name, T &result)
    {
        std::map<std::string, Matprop>::iterator it = this->properties.find(name);
        if(it != this->properties.end())
        	result = it->second.GetValue<T>();
		else
			return false;
		return true;	
    }

    template<typename T>
    T& GetValueRef(const std::string& name)
    {
        std::map<std::string, Matprop>::iterator it = this->properties.find(name);
        if(it == this->properties.end())
		{
			T* val = nullptr;
            return *val;
		}
        return it->second.GetValueRef<T>();
    }

	void ApplyOnlyExist(const std::string& name, std::any val)
	{
		if (!HasValue(name))
			return;
		properties[name].ApplyOverride(val);
	}

	void ApplyExtrinsic(const std::string& name, std::any val)
	{
		if (!HasExtrinsic(name))
			return;
		extrinsic[name].ApplyOverride(val);
	}
};


class Material {

public:
    std::string materialName;
    GLuint programID;

    MaterialProperties properties;

	bool instanced;
    
    Material(const std::string _materialName);
	Material(const Material &copy);

	void CreateProperties(std::initializer_list<MatpropInit> props, std::initializer_list<MatpropInit> exprops)
	{
		for (auto &&i : props)
		{
			GLint location = glGetUniformLocation(programID, i.name.c_str());
			if (location >= 0)
				properties.properties.emplace(i.name, Matprop {i.value, i.type, location});
		}

		for (auto &&i : exprops)
		{
			GLint location = glGetUniformLocation(programID, i.name.c_str());
			if (location >= 0)
				properties.extrinsic.emplace(i.name, Matprop {i.value, i.type, location});
		}
	}

    virtual void Setup();
    virtual void Bind();
};

class VertexFragmentCombinationMaterial : public Material {
    public:
    std::string vertexPath;
    std::string fragmentPath;

    VertexFragmentCombinationMaterial(std::string _materialName, std::string _vertexPath, std::string _fragmentPath);
};