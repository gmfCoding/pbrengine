#include "Cube.hpp"
#include "gpu_mesh.hpp"

PrimitiveCube::PrimitiveCube(/* args */)
{
	BoxMeshGen* bmg = new BoxMeshGen();
    mesh = bmg->generate();

    renderer = new MeshRenderer(MaterialSystem::Copy("basic"), *this, new GPUMesh(&mesh));
    delete bmg;
}

PrimitiveCube::~PrimitiveCube()
{
    delete renderer;
}

void PrimitiveCube::Render()
{
	this->renderer->Render();
}