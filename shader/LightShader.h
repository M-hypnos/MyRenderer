#pragma once
#include "IShader.h"
#include "../entity/Light.h"
class LightShader :
    public IShader
{
public:
    std::vector<Light*> light;
    Vec3f viewPos;
    virtual void vertexShader(const Vertex& input, shaderVert& output);
    virtual Color fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material);
};

