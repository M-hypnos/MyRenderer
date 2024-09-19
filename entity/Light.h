#pragma once
#include "../GlobalConst.h"
#include "../utils/Math.h"
class Light
{
public:
	Light(LightType lightType = LightType::Parallel):lightType_(lightType){}
	void leftPress();
	void rightPress();

	Vec3f direct_;
	Vec3f position_;

	float constant_;
	float linear_;
	float quadratic_;

	float cutOff_;
	float outerCutOff_;

	Vec3f ambient_;
	Vec3f diffuse_;
	Vec3f specular_;

	LightType lightType_;
};

