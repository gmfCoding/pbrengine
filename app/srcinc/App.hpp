#include "PlayerMove.hpp"
#include "BaseApp.hpp"
class GizmoLine;
class PrimitiveCube;

class ExampleApp : public BaseApp
{
public:
	GizmoLine* line1;
	GizmoLine* line2;
	GizmoLine* line3;
    PrimitiveCube* prim_cube;

    ExampleApp();

	void Setup() override;
	void RenderingBegin() override;

	void RenderingEnd() override;
	void Update() override;

};