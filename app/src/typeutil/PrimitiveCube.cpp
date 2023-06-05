#include "Cube.hpp"

PrimitiveCube::PrimitiveCube(/* args */)
{
    renderer = new MeshRenderer(MaterialSystem::Copy("basic"), *this);

    BoxMeshGen* bmg = new BoxMeshGen();
    mesh = bmg->generate();
    renderer->Bind(&mesh);

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