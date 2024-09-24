#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Color.h"

class Texture
{
public:
	const char* filename;
	const char* type;
	Texture() = default;
	bool loadFromImage(const char* filename, const char* type);
	Color getPixel(float u, float v);
	Color getPixel(int x, int y);
private:
	SDL_Surface* surf_;
};

