#include "TestShader.h"
#include "../core/STexturePool.h"

Vec4f TestShader::vertexShader(const Vertex& input) {
	Vec4f v = viewMat4 * modelMat4 * embed<4>(input.pos);
	v = projectMat4 * v;
	return v;
}
SColor TestShader::fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material) {

	Vec2f uv = (v1.texcoords * bc.x + v2.texcoords * bc.y + v3.texcoords * bc.z);
	SColor c = STexturePool::getInstance().getTexture(material.Kd_texIdx).getPixel(uv.x, uv.y);

	return c;
}