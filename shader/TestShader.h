#pragma once
#include "IShader.h"

class TestShader:
	public IShader
{
public:
	virtual Vec4f vertexShader(const Vertex& input);
	virtual SColor fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material);
};

