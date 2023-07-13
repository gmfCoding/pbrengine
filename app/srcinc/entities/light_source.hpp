#include "Rendering.hpp"

class LightSource : public Object
{
private:
    /* data */
    GLuint vao;
	float brightness;

public:
    LightSource(float p_brightness = 0.0f);
    ~LightSource();
};