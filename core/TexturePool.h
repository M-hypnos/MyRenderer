#pragma once
#include "Texture.h"
#include <vector>
#include <string>
class TexturePool
{
public:
	static TexturePool& getInstance();
	Texture getTexture(int idx);
	Texture getTexture(const char* filename);
	int pushToCache(Texture tex);
	int isTextureLoaded(const char* filename);
    TexturePool(const TexturePool&) = delete;
    TexturePool(TexturePool&&) = delete;
    TexturePool& operator=(const TexturePool&) = delete;
    TexturePool& operator=(TexturePool&&) = delete;
private:
    TexturePool();
    ~TexturePool();
    std::vector<Texture> texturePool_;
};

