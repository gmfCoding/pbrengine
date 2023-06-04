#include <vector>
#include <string>
#include <iostream>
#include <glm/vec4.hpp>

#include "glad.h"
#include "Material.hpp"

class PostprocessEffect
{
public:
    std::string name;
    Material* material;
    bool initialised = false;
    bool enabled = false;

    PostprocessEffect(const std::string& pName);

    PostprocessEffect(const std::string& pName, Material* pMaterial);

public:
    virtual void Setup();

    virtual void Update();
};


/// @brief 
/// The game draws mainly to the prefx_fbo then the post processing is applied to the default fbo
class PostProcessing {

public:
    bool effectsEnabled;

public:
    std::vector<PostprocessEffect*> effects;

    PostprocessEffect* noeffect;

    // Full-screen sized quad vertex array object.
    GLuint postfx_vao;
    GLuint prefx_fbo, prefx_rbo, prefx_renderTexture;

    bool enable;

    PostProcessing();

public:
    void Init();

    void Begin();
    void End();

    void Add(PostprocessEffect* effect, int order);
    void ApplyEffects();
    void DrawScreenSizedQuad();
    int ActiveEffectsCount();
};