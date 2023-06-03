#include "PlayerMove.hpp"

class Camera;
class PlayerController;
class PostProcessing;
class PostprocessEffect;

class BaseApp
{
public:
    PostProcessing* postprocess;
    PostprocessEffect* underwater_effect;

	PlayerMove playerMove{};
    Camera* camera;
    PlayerController* playerController;

    int* width;
    int* height;

public:
	BaseApp();
	~BaseApp();

	virtual void Setup();

	virtual void RenderingBegin() = 0;
	virtual void Update() = 0;
	virtual void RenderingEnd() = 0;
	void SetupPostProcessing();
};