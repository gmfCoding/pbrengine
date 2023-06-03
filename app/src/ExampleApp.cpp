#include <glad/glad.h>
#include <glm/gtx/transform.hpp>

#include "App.hpp"
#include "Rendering.hpp"
#include "PlayerController.hpp"
#include "Postprocess.hpp"
#include "GizmoLine.hpp"
#include "Cube.hpp"

ExampleApp::ExampleApp()
{

}

void ExampleApp::Setup()
{
	BaseApp::Setup();
	this->SetupPostProcessing();
	
	camera->SetPosition(glm::vec3(7.0f,2.0f,7.0f));
	camera->UpdateView();

	line1 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(1,0,0));
	line2 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(0,1,0));
	line3 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(0,0,1));
	
	std::cout << glm::vec3(1,0,0).y << std::endl;
	line1->setColor(glm::vec3(1,0,0) + glm::vec3(0));
	line2->setColor(glm::vec3(0,1,0) + glm::vec3(0));
	line3->setColor(glm::vec3(0,0,1) + glm::vec3(0));
}

void ExampleApp::Update()
{
    playerController->Control(playerMove);
    Renderer::camera->UpdateView();
    glm::mat4 feet = Renderer::camera->preMultPV * glm::translate(Renderer::camera->GetPosition()) * glm::translate(glm::vec3(0,-1.6,0));

	this->prim_cube = new PrimitiveCube();

	// Player Feet
	this->line1->setMVP(feet);
	this->line2->setMVP(feet);
	this->line3->setMVP(feet);

	this->line1->draw(Renderer::camera);
	this->line2->draw(Renderer::camera);
	this->line3->draw(Renderer::camera);

	feet = Renderer::camera->preMultPV;
	this->line1->setMVP(feet);
	this->line2->setMVP(feet);
	this->line3->setMVP(feet);

	this->line1->draw(Renderer::camera);
	this->line2->draw(Renderer::camera);
	this->line3->draw(Renderer::camera);
}

void ExampleApp::RenderingBegin()
{
    postprocess->Begin();
}

void ExampleApp::RenderingEnd()
{
    postprocess->End();
    postprocess->ApplyEffects();
}