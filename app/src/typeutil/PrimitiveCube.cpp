#include "Cube.hpp"

PrimitiveCube::PrimitiveCube(/* args */)
{
    renderer = new MeshRenderer("basic");

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
	Renderer::RenderObject(this);
}