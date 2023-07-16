#include "PlayerMove.hpp"
#include "BaseApp.hpp"
#include <vector>

class Object;
class GizmoLine;
class PrimitiveCube;
class GPUManager;

class ShadowtestApp : public BaseApp
{
public:
	Object* crate;
	Object* fwn;
	Object* rounded;
	Object* plane;

	GizmoLine* line1;
    GizmoLine* line2;
    GizmoLine* line3;

	std::vector<Object*> objects;
	Object* light;
	GPUManager* gpuman;

    ShadowtestApp();

	void Setup() override;
	void RenderingBegin() override;

	void RenderingEnd() override;
	void Update() override;

};