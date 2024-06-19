#include "TestShader.h"

void TestShader::vertexShader(Vertex& vert, ShaderVertex& svert) {
	Vec4f v = viewMat4 * modelMat4 * embed<4>(vert.pos);
	svert.worldPos = v;
	v = projectMat4 * v;
	v.homogeneousAlignment();
	svert.ndcPos = v;
}
bool TestShader::fragmentShader(ShaderVertex& svert) {

	svert.color = TColor(255);
	return true;
}