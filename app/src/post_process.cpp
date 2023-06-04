#include <vector>
#include <string>
#include <iostream>
#include <glm/vec4.hpp>

#include "engine.hpp"
#include "Material.hpp"
#include "fileio.hpp"
#include "glad.h"
#include "Postprocess.hpp"

PostprocessEffect::PostprocessEffect(const std::string& pName) : name(pName)
{

}

PostprocessEffect::PostprocessEffect(const std::string& pName, Material* pMaterial) : name(pName), material(pMaterial)
{
	initialised = true;
}

void PostprocessEffect::Setup()
{
	if(material != nullptr)
		initialised = true;
}

void PostprocessEffect::Update()
{
	glUseProgram(material->programID);
}

PostProcessing::PostProcessing() : effects()
{
	noeffect = new PostprocessEffect("no_effect", new VertexFragmentCombinationMaterial("postfx_noeffect", getAssetPath({"shaders", "vertex_uv.shader"}),   getAssetPath({"shaders", "postfx", "noeffect.shader"})));
}

void PostProcessing::Init()
{
	const int &width = currentEngine->width;
	const int &height = currentEngine->height;

	// Set up vertex data (and buffer(s)) and attribute pointers
	static GLfloat vertices[] = {
		// Positions       // Texture Coords
		-1.0f,  1.0f, 0.2f,  0.0f, 1.0f,  // Top-left
		1.0f,  1.0f, 0.2f,  1.0f, 1.0f,  // Top-right
		1.0f, -1.0f, 0.2f,  1.0f, 0.0f,  // Bottom-right
		-1.0f, -1.0f, 0.2f,  0.0f, 0.0f   // Bottom-left
	};
	static GLuint indices[] = {
		0, 3, 1, // First triangle
		1, 3, 2  // Second triangle
	};
	
	GLuint postfx_vbo, postfx_ebo;
	glGenVertexArrays(1, &postfx_vao);
	glGenBuffers(1, &postfx_vbo);
	glGenBuffers(1, &postfx_ebo);

	glBindVertexArray(postfx_vao);

	glBindBuffer(GL_ARRAY_BUFFER, postfx_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, postfx_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	// Set up the framebuffer object that the scene will draw too.
	glGenFramebuffers(1, &prefx_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, prefx_fbo);

	// Create a render buffer object to store the depth buffer
	glGenRenderbuffers(1, &prefx_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, prefx_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, prefx_rbo);

	glGenTextures(1, &prefx_renderTexture);
	glBindTexture(GL_TEXTURE_2D, prefx_renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, prefx_renderTexture, 0);

	// Check if the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::Begin()
{
	effectsEnabled = ActiveEffectsCount() > 0 && enable;
	
	// Render the scene directly to the default buffer
	if(!effectsEnabled)
	{
		End();
	}
	else
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prefx_fbo);

	glEnable(GL_DEPTH_TEST);
	glm::vec4 cc = *currentEngine->clear_color;
	glClearColor(cc.x * cc.w, cc.y * cc.w, cc.z * cc.w, cc.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessing::End()
{
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void PostProcessing::Add(PostprocessEffect* effect, int order)
{
	effects.insert(effects.begin() + order, effect);
	effect->Setup();
}

void PostProcessing::ApplyEffects()
{
	if(!effectsEnabled)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, prefx_renderTexture);
	
	for (auto i : effects)
	{
		if(i->initialised && i->enabled)
			i->Update();
		DrawScreenSizedQuad();
	}
}

void PostProcessing::DrawScreenSizedQuad()
{
	if(!effectsEnabled)
		return;

	glBindVertexArray(postfx_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


int PostProcessing::ActiveEffectsCount()
{
	int active = 0;
	for (auto i : effects)
	{
		if(i->enabled)
			active++;
	}

	return active;
}