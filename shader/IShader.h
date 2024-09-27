#pragma once
//#include "../core/SModel.h"
#include "../core/Obj_Loader.h"
#include "../utils/Math.h"
#include "../core/Color.h"

struct shaderVert
{
	shaderVert() = default;
	shaderVert(Vec4f c):clip_coords(c){}
	shaderVert(Vec4f c, Vec3f n, Vec3f w, Vec3f p, Vec2f t):clip_coords(c), normal(n), world_coords(w), pos(p), texcoords(t){}
	Vec2f texcoords;
	Vec3f normal;
	Vec3f world_coords;
	Vec3f pos;
	Vec4f clip_coords;
	Vec4f screen_coords;
	static shaderVert lerp(const shaderVert& lhs, const shaderVert& rhs, float ratio) {
		Vec2f t = lhs.texcoords + (rhs.texcoords - lhs.texcoords) * ratio;
		Vec3f n = lhs.normal + (rhs.normal - lhs.normal) * ratio;
		Vec3f w = lhs.world_coords + (rhs.world_coords - lhs.world_coords) * ratio;
		Vec3f p = lhs.pos + (rhs.pos - lhs.pos) * ratio;
		Vec4f c = lhs.clip_coords + (rhs.clip_coords - lhs.clip_coords) * ratio;
		return shaderVert(c, n, w, p, t);
	}
};

class IShader
{
public:
	Matrix modelMat4;
	Matrix viewMat4;
	Matrix projectMat4;
	Matrix MVT;
	virtual void vertexShader(const Vertex& v1, const Vertex& v2, const Vertex& v3, std::vector<shaderVert>& output){
		Vec4f mv1 = modelMat4 * embed<4>(v1.pos);
		output.emplace_back(projectMat4 * viewMat4 * mv1, proj<3>(MVT * embed<4>(v1.normal)).normalize(), proj<3>(mv1), v1.pos, v1.texcoords);
		Vec4f mv2 = modelMat4 * embed<4>(v2.pos);
		output.emplace_back(projectMat4 * viewMat4 * mv2, proj<3>(MVT * embed<4>(v2.normal)).normalize(), proj<3>(mv2), v2.pos, v2.texcoords);
		Vec4f mv3 = modelMat4 * embed<4>(v3.pos);
		output.emplace_back(projectMat4 * viewMat4 * mv3, proj<3>(MVT * embed<4>(v3.normal)).normalize(), proj<3>(mv3), v3.pos, v3.texcoords);
	};
	virtual Color fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material) = 0;
};

