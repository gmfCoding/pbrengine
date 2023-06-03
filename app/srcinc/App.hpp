#include "PlayerMove.hpp"


class Camera;
class PlayerController;
class PostProcessing;
class PostprocessEffect;

class App
{
    public:
    static App* instance;

    PlayerMove playerMove{};
    Camera* camera;
    PlayerController* playerController;

    PostProcessing* postprocess;
    PostprocessEffect* underwater_effect;

    int* width;
    int* height;

    App();

    void Start();
    void SetupPostProcessing();

    void Update();  

    void RenderingBegin();
    void RenderingEnd();
};