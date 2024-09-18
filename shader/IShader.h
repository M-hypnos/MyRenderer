#pragma once
//#include "../core/SModel.h"
#include "../core/SObj_loader.h"
#include "../utils/Math.h"
#include "../core/SColor.h"

struct shaderVert
{
	shaderVert() = default;
	shaderVert(Vec4f c):clip_coords(c){}
	shaderVert(Vec4f c, Vec3f n, Vec2f t):clip_coords(c), normal(n), texcoords(t){}
	Vec2f texcoords;
	Vec3f normal;
	Vec4f clip_coords;
	Vec4f screen_coords;
	static shaderVert lerp(const shaderVert& lhs, const shaderVert& rhs, float ratio) {
		Vec2f t = lhs.texcoords + (rhs.texcoords - lhs.texcoords) * ratio;
		Vec3f n = lhs.normal + (rhs.normal - lhs.normal) * ratio;
		Vec4f c = lhs.clip_coords + (rhs.clip_coords - lhs.clip_coords) * ratio;
		return shaderVert(c, n, t);
	}
};

class IShader
{
public:
	Matrix modelMat4;
	Matrix viewMat4;
	Matrix projectMat4;
	virtual Vec4f vertexShader(const Vertex& input) = 0;
	virtual SColor fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material) = 0;
};

