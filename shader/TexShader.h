#pragma once
#include "IShader.h"

class TexShader:
	public IShader
{
public:
	virtual Color fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material);
};

