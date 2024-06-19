#ifndef __SHADER_VERTEX_H__
#define __SHADER_VERTEX_H__

#include "../utils/Geometry.h"
#include "../core/TColor.h"

class ShaderVertex {
public:
	Vec4f worldPos;
	Vec4f ndcPos;
	Vec4f screenPos;
	float depth;

	TColor color;
};

#endif