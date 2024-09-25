#pragma once
#include "IShader.h"

class WhiteShader
	:public IShader
{
	public:
		virtual void vertexShader(const Vertex& input, shaderVert& output);
		virtual Color fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material);
};
