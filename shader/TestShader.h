#ifndef __TEST_SHADER_H__
#define __TEST_SHADER_H__
#include "IShader.h"

class TestShader:public IShader {
public:
	Matrix modelMat4;
	Matrix viewMat4;
	Matrix projectMat4;
	virtual void vertexShader(Vertex& vert, ShaderVertex& svert);
	virtual bool fragmentShader(ShaderVertex& svert);
};

#endif
