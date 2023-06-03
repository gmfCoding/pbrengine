#pragma once
#include <map>
#include <glm/fwd.hpp>
#include <string>

#include "Input.hpp"
#include "CommonData.hpp"

class GLFWwindow;
class BaseApp;
class AABBRenderer;
class CollisionSystem;
class Rigidbody;

class Engine {

    public:
    GLFWwindow* window;
    double timeLastFrame;
    double deltaTime;
    double targetFPS = 60.0;
    bool limitFPS = true;
    bool quit = false;
    
    int width = 800;
    int height = 600;

    bool onKeyUpdate;
    glm::vec4* clear_color;
    Input input;

	BaseApp* app;

    std::map<std::string, RectUV> uvMap;

    CollisionSystem* collisions;
    std::vector<AABBRenderer*> aabbrenderers;
	std::vector<Rigidbody*> rigidbodies;
	bool renderAABBs = false;

	bool hideMouse = true;

	std::map<std::string, BaseApp*> apps;

	
	 bool show_demo_window = false;
    bool show_atlas_tex = false;
    bool gui_show_postprocessing = false;
    bool gui_show_information = true;


	int Intialise();
    virtual void Start();
    virtual void Update();

	void LoadMaterials();

    void ImGUIExample();

    void Input();

	static BaseApp* Get();

    void engine_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

extern Engine *currentEngine;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorpos_callback(GLFWwindow* window, double x, double y);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);