#include "WhiteShader.h"

void WhiteShader::vertexShader(const Vertex& input, shaderVert& output) {
	Vec4f v = modelMat4 * embed<4>(input.pos);
	output.world_coords = proj<3>(v);
	output.clip_coords = projectMat4 * viewMat4 * v;
	output.normal = proj<3>(MVT * embed<4>(input.normal)).normalize();
}
Color WhiteShader::fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material) {
	return Color(255);
}