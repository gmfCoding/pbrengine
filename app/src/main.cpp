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


#include "apps/ExampleApp.hpp"
#include "apps/Shadowtest.hpp"
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


int main(int argc, char **argv) 
{
    Engine engine = Engine();
	engine.apps.emplace("example", new ExampleApp());
	engine.apps.emplace("shadowtest", new ShadowtestApp());
    return engine.Intialise();
}
