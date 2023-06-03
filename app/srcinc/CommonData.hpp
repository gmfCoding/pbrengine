#ifndef COMMONDATA_H_INCLUDE
#define COMMONDATA_H_INCLUDE
#include <glm.hpp>

class RectUV
{
    public:
    glm::vec2 topRight;
    glm::vec2 topLeft;
    glm::vec2 bottomRight;
    glm::vec2 bottomLeft;
    
    RectUV();
    RectUV (glm::vec2 p_topRight, glm::vec2 p_topLeft, glm::vec2 p_bottomRight, glm::vec2 p_bottomLeft, float size);
};

#endif