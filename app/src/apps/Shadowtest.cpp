#include <glad/glad.h>
#include <glm/gtx/transform.hpp>

#include "apps/Shadowtest.hpp"
#include "Rendering.hpp"
#include "PlayerController.hpp"
#include "Postprocess.hpp"
#include "GizmoLine.hpp"
#include "Cube.hpp"
#include "util/ModelImporter.hpp"
#include "fileio.hpp"
#include "Object.hpp"
#include "Rendering.hpp"

ShadowtestApp::ShadowtestApp()
{

}

void ShadowtestApp::Setup()
{
	BaseApp::Setup();
	this->SetupPostProcessing();

	ModelImporter* importer = new ModelImporter();
	Model* model = importer->LoadModel(getAssetPath({"models", "crate.obj"}).c_str());
	this->crate = new Object();
	crate->mesh = new Mesh();
	Model test = model->CreateFlattened();
	model->SetMesh(crate->mesh);
	crate->renderer = new MeshRenderer(MaterialSystem::materialMap["basic"], *crate);
	crate->renderer->m_mesh = crate->mesh;
	crate->renderer->Bind(crate->mesh);
}

void ShadowtestApp::Update()
{
    playerController->Control(playerMove);
    Renderer::camera->UpdateView();
	this->crate->renderer->Render();
}

void ShadowtestApp::RenderingBegin()
{
    postprocess->Begin();
}

void ShadowtestApp::RenderingEnd()
{
    postprocess->End();
    postprocess->ApplyEffects();
}