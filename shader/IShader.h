#pragma once
//#include "../core/SModel.h"
#include "../core/SObj_loader.h"
#include "../utils/Math.h"
#include "../core/SColor.h"

class IShader
{
public:
	Matrix modelMat4;
	Matrix viewMat4;
	Matrix projectMat4;
	virtual Vec4f vertexShader(const Vertex& input) = 0;
	virtual SColor fragmentShader(const Vertex* input, Vec3f bc, Material& material) = 0;
};

