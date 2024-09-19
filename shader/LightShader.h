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
    virtual SColor fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material);
    Vec3f calculateLight(Light* light, Vec3f normal, Vec3f fragPos, Vec3f ka, Vec3f kd, Vec3f ks);
};

