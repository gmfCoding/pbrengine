#include "Rendering.hpp"
#include "MaterialSystem.hpp"
#include "BoxMeshGen.hpp"

class PrimitiveCube : public Object
{
private:
    /* data */
    static GLuint vao;
    Mesh mesh;
public:
    PrimitiveCube(/* args */);
    ~PrimitiveCube();

	void Render();
};