#include "TestShader.h"
#include "../core/STexturePool.h"

Vec4f TestShader::vertexShader(const Vertex& input) {
	Vec4f v = viewMat4 * modelMat4 * embed<4>(input.pos);
	v = projectMat4 * v;
	return v;
}
SColor TestShader::fragmentShader(const Vertex* input, Vec3f bc, Material& material) {

	Vec2f uv = (input[0].texcoords * bc.x + input[1].texcoords * bc.y + input[2].texcoords * bc.z);
	//printf("%f, %f\n", uv.x, uv.y);
	SColor c = STexturePool::getInstance().getTexture(material.Kd_texIdx).getPixel(uv.x, uv.y);

	return c;
}