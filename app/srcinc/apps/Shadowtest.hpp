#include "PlayerMove.hpp"
#include "BaseApp.hpp"

class Object;
class GizmoLine;
class PrimitiveCube;

class ShadowtestApp : public BaseApp
{
public:
	Object* crate;
    ShadowtestApp();

	void Setup() override;
	void RenderingBegin() override;

	void RenderingEnd() override;
	void Update() override;

};