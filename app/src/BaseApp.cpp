#include "Postprocess.hpp"
#include "Camera.hpp"
#include "PlayerMove.hpp"
#include "PlayerController.hpp"
#include "BaseApp.hpp"
#include "Rendering.hpp"
#include "engine.hpp"

BaseApp::BaseApp()
{
}

BaseApp::~BaseApp()
{
}

void BaseApp::Setup()
{
    this->camera = new Camera(70.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    this->playerController = new PlayerController();
    this->playerController->camera = camera;
    this->playerController->eyeposition = camera->PtrPosition();

	this->width = &currentEngine->width;
	this->height = &currentEngine->height;
	Renderer::camera = this->camera;
}

void BaseApp::SetupPostProcessing()
{
    postprocess = new PostProcessing();
    postprocess->Init();
    postprocess->enable = true;

    underwater_effect = new PostprocessEffect("underwater", new VertexFragmentCombinationMaterial("postfx_underwater", getAssetPath({"shaders", "vertex_uv.shader"}),   getAssetPath({"shaders", "postfx", "underwater.shader"})));
    underwater_effect->Setup();
    underwater_effect->enabled = false;

	postprocess->Add(underwater_effect, 0);

}
