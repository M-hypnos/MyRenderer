#include "LightShader.h"
#include "../GlobalConst.h"

void LightShader::vertexShader(const Vertex& input, shaderVert& output) {
	Vec4f v = modelMat4 * embed<4>(input.pos);
	output.world_coords = proj<3>(v);
	output.clip_coords = projectMat4 * viewMat4 * v;
}

Vec3f LightShader::calculateLight(Light* light, Vec3f normal, Vec3f fragPos, Vec3f ka, Vec3f kd, Vec3f ks) {
	if (light->lightType_ == LightType::Parallel) {
		Vec3f lightDir = light->direct_ * (-1);
		lightDir.normalize();
		Vec3f viewDir = (viewPos - fragPos).normalize();
		Vec3f halfVector = (viewDir + lightDir).normalize();

		float diff = std::max(normal * lightDir, 0.f);
		Vec3f diffColor = cwiseProduct(kd, light->diffuse_) * diff;
		Vec3f ambientColor = cwiseProduct(ka, light->ambient_);

		float spec = std::pow(std::max(normal * halfVector, 0.f), 32);
		Vec3f specColor = cwiseProduct(ks, light->specular_) * spec;
		return diffColor + ambientColor + specColor;
	}
	else if (light->lightType_ == LightType::Point) {
		Vec3f lightDir = (light->position_ - fragPos).normalize();
		float distance = (light->position_ - fragPos).norm();

		Vec3f viewDir = (viewPos - fragPos).normalize();
		Vec3f halfVector = (viewDir + lightDir).normalize();

		float attenuation = 1.f / (light->constant_ + light->linear_ * distance + light->quadratic_ * distance * distance);
		attenuation = 1.f;

		float diff = std::max(normal * lightDir, 0.f);
		float spec = std::pow(std::max(normal * halfVector, 0.f), 32);

		Vec3f diffColor = cwiseProduct(kd, light->diffuse_) * diff * attenuation;
		Vec3f ambientColor = cwiseProduct(ka, light->ambient_) * attenuation;
		Vec3f specColor = cwiseProduct(ks, light->specular_) * spec * attenuation;
		return diffColor + ambientColor + specColor;
	}
	else {
		return Vec3f(0.f, 0.f, 0.f);
	}
}

SColor LightShader::fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material) {

	/*Vec2f uv = (v1.texcoords * bc.x + v2.texcoords * bc.y + v3.texcoords * bc.z);
	SColor c = STexturePool::getInstance().getTexture(material.Kd_texIdx).getPixel(uv.x, uv.y);*/

	Vec3f color;

	Vec2f uv = (v1.texcoords * bc.x + v2.texcoords * bc.y + v3.texcoords * bc.z);
	Vec3f n = (v1.normal * bc.x + v2.normal * bc.y + v3.normal * bc.z);
	n.normalize();
	Vec3f fragPos = (v1.world_coords * bc.x + v2.world_coords * bc.y + v3.world_coords * bc.z);

	Vec3f ks = material.Ks;
	Vec3f kd;
	if (material.map_Kd != "") {
		SColor c = STexturePool::getInstance().getTexture(material.Kd_texIdx).getPixel(uv.x, uv.y);
		kd = Vec3f(c.r / 255.f, c.g / 255.f, c.b / 255.f);
	}
	//kd = material.Kd;

	Vec3f ka = kd;

	for (auto l : light) {
		color = color + calculateLight(l, n, fragPos, ka, kd, ks);
	}

	return SColor(color.x * 255, color.y * 255, color.z * 255);
}