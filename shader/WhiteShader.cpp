#include "WhiteShader.h"

Color WhiteShader::fragmentShader(const shaderVert& v1, const shaderVert& v2, const shaderVert& v3, const Vec3f bc, const Material& material) {
	return Color(255);
}