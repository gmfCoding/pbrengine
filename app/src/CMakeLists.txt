cmake_minimum_required(VERSION 3.12)

set(LSOURCE 
main.cpp
ExampleApp.cpp
Camera.cpp
engine.cpp
Input.cpp
main.cpp
Mesh.cpp
Material.cpp
MaterialSystem.cpp
Object.cpp
PlayerController.cpp
Rendering.cpp
GizmoLine.cpp
BoxMeshGen.cpp
TextureManager.cpp
CommonData.cpp
transform.cpp
BaseApp.cpp
post_process.cpp
rendering/line_renderer.cpp
rendering/aabb_renderer.cpp
typeutil/PrimitiveCube.cpp
typeutil/AABBWireframe.cpp
physics/staticbody.cpp
physics/collision_system.cpp
physics/physdata.cpp
physics/physutil.cpp
)

list(TRANSFORM LSOURCE PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

message(STATUS "[LSOURCE/extra] files: ${LSOURCE}")
set(SOURCES ${SOURCES} ${LSOURCE} PARENT_SCOPE)