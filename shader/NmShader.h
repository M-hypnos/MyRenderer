#pragma once
#include "IShader.h"
#include "../entity/Light.h"

class NmShader
   :public IShader
{
public:
    std::vector<Light*> light;
    Vec3f viewPos;
    bool normalMap;
    std::vector<Vec3f> lightPos;
    virtual void vertexShader(const Vertex& v1, const Vertex& v2, const Vertex& v3, std::vector<shaderVert>& output) override;
    virtual Color fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material);
};

