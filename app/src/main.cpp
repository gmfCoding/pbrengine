#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <vector>
#include <set>

#include <stb_image.h>

#include "Input.hpp"
#include "Mesh.hpp"
#include "Object.hpp"
#include "Camera.hpp"
#include "Rendering.hpp"
#include "gldebug.hpp"
#include "engine.hpp"
#include "PlayerMove.hpp"
#include "PlayerController.hpp"
#include "MaterialSystem.hpp"
#include "Material.hpp"
#include "fileio.hpp"
#include "gmfc_image.hpp"

#include "TextureManager.hpp"
#include "ImageExample.hpp"


#include "App.hpp"

#include "Postprocess.hpp"

#include "GizmoLine.hpp"
#include "Cube.hpp"

#include "physics/collision_system.hpp"
#include "typeutil/AABBWireframe.hpp"
#include "rendering/line_renderer.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rendering/aabb_renderer.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

class BasicApp : public Engine {

    App* mc;

    GizmoLine* line1;
    GizmoLine* line2;
    GizmoLine* line3;

    ImageExample ie;

    std::map<std::string, RectUV> uvMap;
    PrimitiveCube* prim_cube;

    CollisionSystem collisions;
    std::vector<AABBRenderer*> aabbrenderers;
	std::vector<Rigidbody*> rigidbodies;

    void Start() override {
        Engine::Start();

        this->clear_color = new glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

        targetFPS = 144.0;

        limitFPS = true;
        glfwSwapInterval(0);

        LoadMaterials();
        mc = new App();
        mc->Start();
        mc->width = &currentEngine->width;
        mc->height = &currentEngine->height;
        mc->SetupPostProcessing();

        Renderer::camera = mc->camera;
        mc->camera->SetPosition(glm::vec3(7.0f,2.0f,7.0f));
        mc->camera->UpdateView();

        line1 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(1,0,0));
        line2 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(0,1,0));
        line3 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(0,0,1));
        
        std::cout << glm::vec3(1,0,0).y << std::endl;
        line1->setColor(glm::vec3(1,0,0) + glm::vec3(0));
        line2->setColor(glm::vec3(0,1,0) + glm::vec3(0));
        line3->setColor(glm::vec3(0,0,1) + glm::vec3(0));


        input.onMouseChangedArr.push_back([this](void* _input){ mc->playerController->OnMouseInput(_input);});

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

        #define TEXTUREPATH(i) getAssetPathMany({"textures",i})

        mc->postprocess->Add(mc->underwater_effect, 0);

        prim_cube = new PrimitiveCube();

		collisions = CollisionSystem{};
        {
            auto sb1 = new Staticbody();
            sb1->aabb = new AABB(glm::vec3(9,3,9));
            *sb1->position = {0,-3,0};
            sb1->RecalculateAABB();
            collisions.Add(sb1);
			aabbrenderers.push_back(new AABBRenderer(sb1->aabb));
        }

        {
            auto sb1 = new Rigidbody();
			rigidbodies.push_back(sb1);
            sb1->aabb = new AABB({0.6, 0.6, 0.6});
            *sb1->position = {0,0.4f,0};
            sb1->RecalculateAABB();
            collisions.Add(sb1);
			aabbrenderers.push_back(new AABBRenderer(sb1->aabb));
        }

		for (auto i : aabbrenderers)
		{
			i->Bind();
		}
    }


    /*
        Setup block texture side definitions: DONE
        Loop over all textures 'pragma once'
        Create an atlas map of them mapping the filename to UV coords
        Foreach block setup the UV Coords that the face texture corrasponds to.
    */
    void LoadMaterials()
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

    void Update() override {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Input();

        glm::mat4 feet = Renderer::camera->preMultPV * glm::translate(Renderer::camera->GetPosition()) * glm::translate(glm::vec3(0,-1.6,0));


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
			collisions.ApplyCollisions();
		}

        mc->RenderingBegin();

        mc->Update();

		for (auto i : aabbrenderers)
		{
			i->Upload();
			i->Render();
		}

        // Player Feet
        line1->setMVP(feet);
        line2->setMVP(feet);
        line3->setMVP(feet);

        line1->draw(Renderer::camera);
        line2->draw(Renderer::camera);
        line3->draw(Renderer::camera);


        // 0,0,0 Origin
        feet = Renderer::camera->preMultPV;
        line1->setMVP(feet);
        line2->setMVP(feet);
        line3->setMVP(feet);

        line1->draw(Renderer::camera);
        line2->draw(Renderer::camera);
        line3->draw(Renderer::camera);

        
        mc->RenderingEnd();

        ImGUIExample();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); 
    }

    bool show_demo_window = false;
    bool show_atlas_tex = false;
    bool gui_show_postprocessing = false;
    bool gui_show_information = true;

    RectUV genRect = RectUV(glm::vec2(0.0, 0.0), glm::vec2(0.0, 1.0), glm::vec2(1.0, 1.0), glm::vec2(1.0, 0.0), 1.0);
    void ImGUIExample()
    {
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
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
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Information", &gui_show_information);
            if (ImGui::Button("View Atlas"))
                show_atlas_tex = true;
            
            Material* mat = MaterialSystem::materialMap["alt_textured_debug"];
            bool &dsp = mat->defaults.GetValueRef<bool>("dsp");
            ImGui::Checkbox("DSP:", &dsp);
            ImGui::InputFloat2("topRight", &genRect.topRight[0]);
            ImGui::InputFloat2("topLeft", &genRect.topLeft[0]);
            ImGui::InputFloat2("bottomRight", &genRect.bottomRight[0]);
            ImGui::InputFloat2("bottomLeft", &genRect.bottomLeft[0]);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
        }



        if(gui_show_postprocessing)
        {
            ImGui::Begin("Post Processing", &gui_show_postprocessing);
            ImGui::Checkbox("PostProcessing:", &(mc->postprocess->enable));
            ImGui::Checkbox("Underwater:", &(mc->underwater_effect->enabled));
            ImGui::Text("Doing Effects:%i", mc->postprocess->effectsEnabled);
            ImGui::Text("Active Effects:%i", mc->postprocess->ActiveEffectsCount());
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_atlas_tex)
        {
            ImGui::Begin("World atlas texture", &show_atlas_tex);
            GLuint atlastex_id = TextureManager::GetNamedTexture("worldatlas");
            ImGui::Image((void*)(intptr_t)atlastex_id, ImVec2(48*4, 48*4));

            if (ImGui::Button("Close Me"))
                show_atlas_tex = false;
            ImGui::End();
        }
    }

    bool hideMouse = true;
    void Input()
    {
        if(onKeyUpdate)
        {
#define KEYCONTROL(keyId, bvar) if(input.isKeyHeld(KeyCode::KEY_##keyId)) { bvar = true;} else{bvar = false;}
            KEYCONTROL(A,mc->playerMove.Left)
            KEYCONTROL(S,mc->playerMove.Backwards)
            KEYCONTROL(D,mc->playerMove.Right)
            KEYCONTROL(W,mc->playerMove.Forward)
            KEYCONTROL(Q,mc->playerMove.Down)
            KEYCONTROL(E,mc->playerMove.Up)


            if(input.isKeyDown(KeyCode::KEY_ESCAPE))
            {
                hideMouse = !hideMouse;

                glfwSetInputMode(window, GLFW_CURSOR, hideMouse ? GLFW_CURSOR_DISABLED: GLFW_CURSOR_NORMAL); 
                input.MouseIgnoreNextDelta();
            }
        }
    }
};


int main() {
    BasicApp engine = BasicApp();
    return engine.Intialise();
}
