#include "Light.h"

void Light::leftPress() {
	switch (lightType_)
	{
	case LightType::Point:
		position_ = position_ + Vec3f(-1, 0, 0);
		break;
	case LightType::Parallel:
		direct_ = direct_ + Vec3f(-1, 0, 0);
		break;
	case LightType::Spot:
		direct_ = direct_ + Vec3f(-1, 0, 0);
		break;
	default:
		break;
	}
}

void Light::rightPress() {
	switch (lightType_)
	{
	case LightType::Point:
		position_ = position_ + Vec3f(1, 0, 0);
		break;
	case LightType::Parallel:
		direct_ = direct_ + Vec3f(1, 0, 0);
		break;
	case LightType::Spot:
		direct_ = direct_ + Vec3f(1, 0, 0);
		break;
	default:
		break;
	}
}