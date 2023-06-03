#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GL/gl.h>
#include <iostream>
#include <string>

#include "engine.hpp" 
#include "Rendering.hpp"
#include "BaseApp.hpp"
#include "Material.hpp"
#include "MaterialSystem.hpp"
#include "physics/collision_system.hpp"
#include "typeutil/AABBWireframe.hpp"
#include "rendering/aabb_renderer.hpp"
#include "Postprocess.hpp"
#include "PlayerController.hpp"

#define def_getcursorposfun
typedef void (* getcursorposfun)(GLFWwindow*, double*,double*);
Engine *currentEngine;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    currentEngine->engine_key_callback(window, key, scancode, action, mods);
}

void cursorpos_callback(GLFWwindow* window, double x, double y)
{
    currentEngine->input.OnMousePosChanged(window, x, y, glfwGetCursorPos);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Engine::engine_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key >= INPUT_KEYCOUNT || action == GLFW_REPEAT)
    {
        return;
    }
    
    onKeyUpdate = true;

    input.OnKeyboardCallback((KeyCode)key, action == GLFW_PRESS ? KeyState::DOWN : action == GLFW_RELEASE ? KeyState::UP : KeyState::NONE);
}

void Engine::LoadMaterials()
{
	auto basic = new VertexFragmentCombinationMaterial("basic",          getAssetPath({"shaders", "basic_vertex.shader"}),   getAssetPath({"shaders", "basic_fragment.shader"}));
	basic->defaults.properties["color"] = glm::vec3(1,1,1);
	
	new VertexFragmentCombinationMaterial("alt_textured",   getAssetPath({"shaders", "alt_tex_vertex.shader"}), getAssetPath({"shaders", "alt_tex_fragment.shader"}));
	auto alttexdebug = new VertexFragmentCombinationMaterial("alt_textured_debug",   getAssetPath({"shaders", "alt_tex_vertex_debug.shader"}), getAssetPath({"shaders", "alt_tex_fragment_debug.shader"}));
	alttexdebug->defaults.properties["dsp"] = false;
	new VertexFragmentCombinationMaterial("default",        getAssetPath({"shaders", "vertex.shader"}),         getAssetPath({"shaders", "fragment.shader"}));
	auto postfx = new VertexFragmentCombinationMaterial("postfx", getAssetPath({"shaders", "vertex_uv.shader"}), getAssetPath({"shaders", "postfx", "hueshift.shader"}));
	postfx->defaults.properties["hueShift"] = 0.0f;

	auto postfx_fxaa = new VertexFragmentCombinationMaterial("postfx_fxaa", getAssetPath({"shaders", "vertex_uv.shader"}), getAssetPath({"shaders", "postfx", "fxaa.shader"}));
	postfx_fxaa->defaults.properties["hueShift"] = 0.0f;
}

BaseApp* Engine::Get()
{
	return currentEngine->app;
}

void Engine::Start() {
	this->clear_color = new glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

	targetFPS = 144.0;

	limitFPS = true;
	glfwSwapInterval(0);
	LoadMaterials();
	
	input.onMouseChangedArr.push_back([this](void* _input){ this->app->playerController->OnMouseInput(_input);});

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	#define TEXTUREPATH(i) getAssetPathMany({"textures",i})


	collisions = new CollisionSystem{};
	{
		auto sb1 = new Staticbody();
		sb1->aabb = new AABB(glm::vec3(9,3,9));
		*sb1->position = {0,-3,0};
		sb1->RecalculateAABB();
		collisions->Add(sb1);
		aabbrenderers.push_back(new AABBRenderer(sb1->aabb));
	}

	{
		auto sb1 = new Rigidbody();
		rigidbodies.push_back(sb1);
		sb1->aabb = new AABB({0.6, 0.6, 0.6});
		*sb1->position = {0,0.4f,0};
		sb1->RecalculateAABB();
		collisions->Add(sb1);
		aabbrenderers.push_back(new AABBRenderer(sb1->aabb));
	}

	for (auto i : aabbrenderers)
	{
		i->Bind();
	}
}


int Engine::Intialise()
{
	currentEngine = this;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(800, 600, "App", NULL, NULL);
    if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursorpos_callback);  
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui_ImplGlfw_InitForOpenGL(window, true);
 
    glEnable(GL_CULL_FACE); 
    glViewport(0, 0, this->width, this->width);
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback( MessageCallback, nullptr);

    double lasttime = glfwGetTime();
	bool appSelected = false;

    while(!glfwWindowShouldClose(window) && !quit) {
        input.Refresh();
        deltaTime =  glfwGetTime() - timeLastFrame;
        timeLastFrame = glfwGetTime();
		if (appSelected)
		{
			this->Update();
		}
		else
		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("App Selector");
			for (auto i : apps)
			{
				if(!ImGui::Button(i.first.c_str()))
					continue;
				appSelected = true;
				app = i.second;
				this->Start();
				app->Setup();
			}
			ImGui::End();
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window); 
		}
        while (limitFPS && glfwGetTime() < lasttime + 1.0/targetFPS) {
            // TODO: Put the thread to sleep, yield, or simply do nothing
        }
        
        lasttime += 1.0/targetFPS;

        glfwPollEvents();  
    }
    glfwTerminate();
    return 0;
}


void Engine::ImGUIExample()
{
		if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("Panels");
	if(ImGui::Button("Information"))
		gui_show_information = true;
	if(ImGui::Button("Post Processing"))
		gui_show_postprocessing = true;

	ImGui::End();

	if(gui_show_information)
	{
		ImGui::Begin("Information", &gui_show_information);
		
		Material* mat = MaterialSystem::materialMap["alt_textured_debug"];
		bool &dsp = mat->defaults.GetValueRef<bool>("dsp");
		ImGui::Checkbox("DSP:", &dsp);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	if(gui_show_postprocessing)
	{
		ImGui::Begin("Post Processing", &gui_show_postprocessing);
		ImGui::Checkbox("PostProcessing:", &(app->postprocess->enable));
		ImGui::Checkbox("Underwater:", &(app->underwater_effect->enabled));
		ImGui::Text("Doing Effects:%i", app->postprocess->effectsEnabled);
		ImGui::Text("Active Effects:%i", app->postprocess->ActiveEffectsCount());
		ImGui::End();
	}
}

void Engine::Update()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	Input();

	for (auto &&i : rigidbodies)
	{
		i->velocity -= glm::vec3(0, 9.81 * deltaTime, 0);
		*(i->position) += (i->velocity * (float)deltaTime);
	}

	for (size_t i = 0; i < 5; i++)
	{
		for (auto &&i : rigidbodies)
		{
			*(i->position) += (i->velocity / 5.0f * (float)deltaTime);
			i->RecalculateAABB();
		}
		collisions->ApplyCollisions();
	}

	app->RenderingBegin();
	app->Update();
	if (renderAABBs)
	{
		for (auto i : aabbrenderers)
		{
			i->Upload();
			i->Render();
		}
	}
	app->RenderingEnd();

	ImGUIExample();
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window); 
}

void Engine::Input()
{
	if(onKeyUpdate)
	{
#define KEYCONTROL(keyId, bvar) if(input.isKeyHeld(KeyCode::KEY_##keyId)) { bvar = true;} else{bvar = false;}
		KEYCONTROL(A, app->playerMove.Left)
		KEYCONTROL(S, app->playerMove.Backwards)
		KEYCONTROL(D, app->playerMove.Right)
		KEYCONTROL(W, app->playerMove.Forward)
		KEYCONTROL(Q, app->playerMove.Down)
		KEYCONTROL(E, app->playerMove.Up)


		if(input.isKeyDown(KeyCode::KEY_ESCAPE))
		{
			hideMouse = !hideMouse;

			glfwSetInputMode(window, GLFW_CURSOR, hideMouse ? GLFW_CURSOR_DISABLED: GLFW_CURSOR_NORMAL); 
			input.MouseIgnoreNextDelta();
		}
	}
}