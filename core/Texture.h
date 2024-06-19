#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include <string>
#include "TColor.h"
#include <SDL.h>
#include <SDL_image.h>

class Texture {
public:
	std::string type;
	std::string path;
	int width;
	int height;
	bool loadFromImage(std::string Path, std::string Type);
	TColor getPixel(float u, float v);
private:
	SDL_Surface* loadedSurface;
};

#endif