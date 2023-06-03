class Camera;
#define OMC4PlayerControllerImGUI

class PlayerController {
    public:
    Camera* camera;

    float height = 1.6f;
    glm::vec3* eyeposition;
	bool applyPlayerGravity;

#ifdef OMC4PlayerControllerImGUI
	bool controllerGuiActive = true;
#endif

    void Control(PlayerMove move);
    void OnMouseInput(void* _input);

    void ProcessMovement();
};
