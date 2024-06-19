#ifndef __ISHADER_H__
#define __ISHADER_H__
#include "../core/Mesh.h"
#include "../core/ShaderVertex.h"
#include "../utils/Geometry.h"
class IShader {
public:
	virtual void vertexShader(Vertex& vert, ShaderVertex& svert) = 0;
	virtual bool fragmentShader(ShaderVertex& svert) = 0;
};

#endif
