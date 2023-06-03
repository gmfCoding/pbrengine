#pragma once
#include "Input.hpp"
#include <glm/fwd.hpp>
class GLFWwindow;

class Engine {

    public:
    int Intialise();
    virtual void Start() = 0;
    virtual void Update() = 0;


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

    void engine_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

extern Engine *currentEngine;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorpos_callback(GLFWwindow* window, double x, double y);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);