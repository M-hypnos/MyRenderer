#include "LightShader.h"
#include "../GlobalConst.h"

void LightShader::vertexShader(const Vertex& input, shaderVert& output) {
	Vec4f v = modelMat4 * embed<4>(input.pos);
	output.world_coords = proj<3>(v);
	output.clip_coords = projectMat4 * viewMat4 * v;
	output.normal = proj<3>(MVT * embed<4>(input.normal)).normalize();
}

Color LightShader::fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material) {

	/*Vec2f uv = (v1.texcoords * bc.x + v2.texcoords * bc.y + v3.texcoords * bc.z);
	Color c = TexturePool::getInstance().getTexture(material.Kd_texIdx).getPixel(uv.x, uv.y);*/

	Vec3f color;

	Vec2f uv = (v1.texcoords * bc.x + v2.texcoords * bc.y + v3.texcoords * bc.z);
	Vec3f n = (v1.normal * bc.x + v2.normal * bc.y + v3.normal * bc.z);
	n.normalize();
	Vec3f fragPos = (v1.world_coords * bc.x + v2.world_coords * bc.y + v3.world_coords * bc.z);

	Vec3f ks = material.Ks;
	if (material.map_Ks != "") {
		Color c = TexturePool::getInstance().getTexture(material.Ks_texIdx).getPixel(uv.x, uv.y);
		ks = Vec3f(c.r / 255.f, c.g / 255.f, c.b / 255.f);
	}
	Vec3f kd = material.Kd;
	if (material.map_Kd != "") {
		Color c = TexturePool::getInstance().getTexture(material.Kd_texIdx).getPixel(uv.x, uv.y);
		kd = Vec3f(c.r / 255.f, c.g / 255.f, c.b / 255.f);
	}

	Vec3f ka = material.Ka;
	if (material.map_Ka != "") {
		Color c = TexturePool::getInstance().getTexture(material.Ka_texIdx).getPixel(uv.x, uv.y);
		ka = Vec3f(c.r / 255.f, c.g / 255.f, c.b / 255.f);
	}else if (material.map_Kd != "") {
		Color c = TexturePool::getInstance().getTexture(material.Kd_texIdx).getPixel(uv.x, uv.y);
		ka = Vec3f(c.r / 255.f, c.g / 255.f, c.b / 255.f);
	}

	Vec3f c;
	for (auto l : light) {
		Vec3f lightDir = (l->position - fragPos).normalize();
		float distance = (l->position - fragPos).norm();

		float diff = std::max(n * lightDir, 0.f);
		Vec3f lightIntensity = l->intensity / distance / distance;
		c = c + cwiseProduct(kd, lightIntensity) * diff;

		Vec3f viewDir = (viewPos - fragPos).normalize();
		Vec3f halfVector = (viewDir + lightDir).normalize();

		float spec = std::pow(std::max(n * halfVector, 0.f), l->p);

		c = c + cwiseProduct(ks, lightIntensity) * spec;

		c = c + cwiseProduct(ka, l->ambient);
	}

	return Color(std::min(c.x, 1.f) * 255, std::min(c.y, 1.f) * 255, std::min(c.z, 1.f) * 255);
}