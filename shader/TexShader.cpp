#include "TexShader.h"
#include "../core/TexturePool.h"

void TexShader::vertexShader(const Vertex& input, shaderVert& output) {
	Vec4f v = modelMat4 * embed<4>(input.pos);
	output.world_coords = proj<3>(v);
	output.clip_coords = projectMat4 * viewMat4 * v;
	output.normal = proj<3>(MVT * embed<4>(input.normal)).normalize();
}
Color TexShader::fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material) {

	Vec2f uv = (v1.texcoords * bc.x + v2.texcoords * bc.y + v3.texcoords * bc.z);
	Color c = TexturePool::getInstance().getTexture(material.Kd_texIdx).getPixel(uv.x, uv.y);

	return c;
}