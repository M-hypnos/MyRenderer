#include "Texture.h"

bool Texture::loadFromImage(const char* filename, const char* type) {
	this->filename = filename;
	this->type = type;
	surf_ = SDL_ConvertSurfaceFormat(IMG_Load(filename), SDL_PIXELFORMAT_RGBA32, 0);
	if (surf_ == nullptr) {
		SDL_Log("Unable to load image %s! SDL_image Error: %s\n", filename, IMG_GetError());
		return false;
	}
	return true;
}

Color Texture::getPixel(float u, float v) {
	if (u < 0 || u > 1 || v < 0 || v > 1) return Color(0);
	return getPixel(int(u * surf_->w), int(v * surf_->h));
}

Color Texture::getPixel(int x, int y) {
	Uint32* pixel = (Uint32*)((Uint8*)surf_->pixels + (surf_->h - y) * surf_->pitch + x * surf_->format->BytesPerPixel);
	Uint8 r, g, b, a;
	SDL_GetRGBA(*pixel, surf_->format, &r, &g, &b, &a);
	return Color(r, g, b, a);
}