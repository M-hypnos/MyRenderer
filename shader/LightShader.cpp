#include "LightShader.h"
#include "../GlobalConst.h"

void LightShader::vertexShader(const Vertex& v1, const Vertex& v2, const Vertex& v3, std::vector<shaderVert>& output) {
	Vec4f mv1 = modelMat4 * embed<4>(v1.pos);
	output.emplace_back(projectMat4 * viewMat4 * mv1, proj<3>(MVT * embed<4>(v1.normal)).normalize(), proj<3>(mv1), v1.pos, v1.texcoords);
	Vec4f mv2 = modelMat4 * embed<4>(v2.pos);
	output.emplace_back(projectMat4 * viewMat4 * mv2, proj<3>(MVT * embed<4>(v2.normal)).normalize(), proj<3>(mv2), v2.pos, v2.texcoords);
	Vec4f mv3 = modelMat4 * embed<4>(v3.pos);
	output.emplace_back(projectMat4 * viewMat4 * mv3, proj<3>(MVT * embed<4>(v3.normal)).normalize(), proj<3>(mv3), v3.pos, v3.texcoords);

	/*if (normalMap) {
		mat<3, 3, float> mat;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				mat[i][j] = modelMat4[i][j];
			}
		}
		mat = mat.invert_transpose();

		Vec3f e1 = v2.pos - v1.pos;
		Vec3f e2 = v3.pos - v1.pos;
		float du1 = v2.texcoords.x - v1.texcoords.x;
		float dv1 = v2.texcoords.y - v1.texcoords.y;
		float du2 = v3.texcoords.x - v1.texcoords.x;
		float dv2 = v2.texcoords.y - v1.texcoords.y;
		float f = 1.f / (du1 * dv2 - du2 * dv1);
		Vec3f T = (e1 * dv2 - e2 * dv1) * f;
		T.normalize();
		Vec3f B = (e2 * du1 - e1 * du2) * f;
		B.normalize();
		T = mat * T;
		T.normalize();
		B = mat * B;
		B.normalize();
		mat<3, 3, float> TBN;
		TBN.set_col(0, T);
		TBN.set_col(1, B);
		TBN.set_col(2, N);

		for (auto l : light) {
			lightPos.emplace_back(l->position);
		}
	}
	else {
		for (auto l : light) {
			lightPos.emplace_back(l->position);
		}
	}*/
	for (auto l : light) {
		lightPos.emplace_back(l->position);
	}
};

Color LightShader::fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material) {
	Vec3f color;

	Vec2f uv = (v1.texcoords * bc.x + v2.texcoords * bc.y + v3.texcoords * bc.z);
	Vec3f n = (v1.normal * bc.x + v2.normal * bc.y + v3.normal * bc.z);
	n.normalize();

	if (material.map_bump != "" && normalMap) {
		float du1 = v2.texcoords.x - v1.texcoords.x;
		float dv1 = v2.texcoords.y - v1.texcoords.y;
		float du2 = v3.texcoords.x - v1.texcoords.x;
		float dv2 = v3.texcoords.y - v1.texcoords.y;
		Vec3f e1 = v2.world_coords - v1.world_coords;
		Vec3f e2 = v3.world_coords - v1.world_coords;
		float f = 1.f / (du1 * dv2 - du2 * dv1);
		Vec3f T = (e1 * dv2 - e2 * dv1) * f;
		T = T - n * (T * n);
		T.normalize();
		Vec3f B = cross(n, T).normalize();
		mat<3, 3, float> TBN;
		TBN.set_col(0, T);
		TBN.set_col(1, B);
		TBN.set_col(2, n);


		Color c = TexturePool::getInstance().getTexture(material.bump_texIdx).getPixel(uv.x, uv.y);
		Vec3f normal = Vec3f(c.r / 255.f, c.g / 255.f, c.b / 255.f);
		normal = (normal * 2.f - Vec3f(1.f, 1.f, 1.f)).normalize();

		n = TBN * normal;
	}

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
	for (int i = 0; i < light.size();i++) {
		Vec3f lightDir = (lightPos[i] - fragPos).normalize();
		float distance = (lightPos[i] - fragPos).norm();

		float diff = std::max(n * lightDir, 0.f);
		Vec3f lightIntensity = light[i]->intensity / distance / distance;
		c = c + cwiseProduct(kd, lightIntensity) * diff;

		Vec3f viewDir = (viewPos - fragPos).normalize();
		Vec3f halfVector = (viewDir + lightDir).normalize();

		float spec = std::pow(std::max(n * halfVector, 0.f), light[i]->p);

		c = c + cwiseProduct(ks, lightIntensity) * spec;

		c = c + cwiseProduct(ka, light[i]->ambient);
	}

	return Color(std::min(c.x, 1.f) * 255, std::min(c.y, 1.f) * 255, std::min(c.z, 1.f) * 255);
}