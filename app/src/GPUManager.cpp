#include "gpu_mesh.hpp"
#include "Mesh.hpp"

#include "GPUManager.hpp"
#include "util/Model.hpp"
#include <iostream>

GPUMesh* GPUManager::GetMesh(std::string const &str)
{
	return this->meshes[str];
}

GPUMesh* GPUManager::AddMesh(std::string const &name, GPUMesh *mesh)
{
	this->meshes.emplace(name, mesh);
	return this->meshes[name];
}

GPUMesh* GPUManager::CreateMesh(std::string const &name, Mesh *mesh)
{	
	return AddMesh(name, new GPUMesh(mesh));
}

GPUMesh* GPUManager::CreateMesh(std::string const &name, Model *model)
{	
	auto m = new Mesh();
	model->SetMesh(m);
	auto gm = AddMesh(name, new GPUMesh(m));
	std::cout << "Tracking new GPUMesh: \"" << name << "\" at VAO:" << gm->GetSpec().vao << std::endl;
	delete m;
	return gm;
}