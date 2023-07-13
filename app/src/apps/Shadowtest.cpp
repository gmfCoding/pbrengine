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
#include "entities/light_source.hpp"
#include "GPUManager.hpp"
#include "engine.hpp"

ShadowtestApp::ShadowtestApp()
{

}


Object *CreateNewObject(std::string const &model, glm::vec3 pos, glm::vec3 color, GPUManager *gpuman)
{
	auto obj = new Object();
	obj->SetPosition(pos);
	obj->renderer = new MeshRenderer(MaterialSystem::Copy("lit"), *obj, gpuman->GetMesh(model));
	obj->renderer->material->properties.SetValue("objectColor", color);

	return obj;
}

void ShadowtestApp::Setup()
{
	BaseApp::Setup();
	this->SetupPostProcessing();

	ModelImporter* importer = new ModelImporter();
	Model* model = importer->LoadModel(getAssetPath({"models", "plane.obj"}).c_str());
	Model* model2 = importer->LoadModel(getAssetPath({"models", "uvsphere.obj"}).c_str());
	Model* model3 = importer->LoadModel(getAssetPath({"models", "cube_fwn.obj"}).c_str());
	Model* model4 = importer->LoadModel(getAssetPath({"models", "cube_split.obj"}).c_str());
	Model* model5 = importer->LoadModel(getAssetPath({"models", "cube_rounded.obj"}).c_str());

	gpuman = new GPUManager();

	gpuman->CreateMesh("plane", model);
	gpuman->CreateMesh("uvsphere", model2);
	gpuman->CreateMesh("cube_fwn", model3);
	gpuman->CreateMesh("cube_flat", model4);
	gpuman->CreateMesh("cube_rounded", model5);

	this->light = new LightSource();
	light->renderer = new MeshRenderer(MaterialSystem::materialMap["lit_source"], *light, gpuman->GetMesh("uvsphere"));
	light->SetPosition(glm::vec3(-3, 2, 1));

	this->crate = CreateNewObject("cube_flat", glm::vec3(3.0,  0.0, 7.0), glm::vec3(1.0, 1.0, 0.0), gpuman);
	this->plane = CreateNewObject("plane", glm::vec3(6.0, 0.0, -1.0),  glm::vec3(1.0, 1.0, 1.0), gpuman);
	this->fwn = CreateNewObject("cube_fwn", glm::vec3(2.0,  0.0, 5.0), glm::vec3(1.0, 1.0, 0.0), gpuman);
	this->rounded = CreateNewObject("cube_rounded", glm::vec3(4.0,  0.0, -3.0), glm::vec3(1.0, 1.0, 0.0), gpuman);
	objects.push_back(crate);
	objects.push_back(plane);
	objects.push_back(fwn);
	objects.push_back(rounded);
}

void ShadowtestApp::Update()
{
	static float angle = 0.0;

	angle += currentEngine->deltaTime;



	float angleX = sin(angle);
	float angleY = cos(angle * 0.5);
	float angleZ = cos(angle);
    playerController->Control(playerMove);
    Renderer::camera->UpdateView();
	this->light->SetPosition(glm::vec3(angleX * 10, angleY * 10, angleZ * 10));

	for(auto var : objects)
	{
		var->renderer->material->properties.SetValue("lightPos", light->GetPosition());
		var->renderer->Render();
	}
	this->light->renderer->Render();
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