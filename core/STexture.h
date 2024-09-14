#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "SColor.h"

class STexture
{
public:
	const char* filename;
	const char* type;
	STexture() = default;
	bool loadFromImage(const char* filename, const char* type);
	SColor getPixel(float u, float v);
	SColor getPixel(int x, int y);
private:
	SDL_Surface* surf_;
};

