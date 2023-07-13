#define MATP_MVP_ONE {"MVP", Matprop::MAT4, glm::mat4(1)}
#define MATP_MVP_SPLIT {"model", Matprop::MAT4, glm::mat4(1)}, \
{"proj", Matprop::MAT4, glm::mat4(1)}, \
{"view", Matprop::MAT4, glm::mat4(1)}

#define MATP_VIEWPOS {"viewPos", Matprop::FLOAT3, glm::vec3(1)}