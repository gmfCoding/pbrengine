#include <glad/glad.h>

#include "App.hpp"
#include "Rendering.hpp"
#include "PlayerController.hpp"
#include "Postprocess.hpp"

App* App::instance;

App::App()
{
    if(!instance)
        instance = this;
}

void App::Start()
{
    camera = new Camera(70.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    playerController = new PlayerController();
    playerController->camera = camera;
    playerController->eyeposition = camera->PtrPosition();
}

void App::SetupPostProcessing()
{
    postprocess = new PostProcessing();
    postprocess->Init();
    postprocess->enable = true;

    underwater_effect = new PostprocessEffect("underwater", new VertexFragmentCombinationMaterial("postfx_underwater", getAssetPath({"shaders", "vertex_uv.shader"}),   getAssetPath({"shaders", "postfx", "underwater.shader"})));
    underwater_effect->Setup();
    underwater_effect->enabled = false;
}

void App::Update()
{
    playerController->Control(playerMove);
    Renderer::camera->UpdateView();
    // world->renderer->Render();
    // worldtest->renderer->Render();
}



void App::RenderingBegin()
{
    postprocess->Begin();
}

void App::RenderingEnd()
{
    postprocess->End();
    postprocess->ApplyEffects();
}