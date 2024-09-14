#pragma once
#include "IShader.h"

class TestShader:
	public IShader
{
public:
	virtual Vec4f vertexShader(const Vertex& input);
	virtual SColor fragmentShader(const Vertex* input, Vec3f bc, Material& material);
};

