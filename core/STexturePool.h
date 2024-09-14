#pragma once
#include "STexture.h"
#include <vector>
#include <string>
class STexturePool
{
public:
	static STexturePool& getInstance();
	STexture getTexture(int idx);
	STexture getTexture(const char* filename);
	int pushToCache(STexture tex);
	int isTextureLoaded(const char* filename);
    STexturePool(const STexturePool&) = delete;
    STexturePool(STexturePool&&) = delete;
    STexturePool& operator=(const STexturePool&) = delete;
    STexturePool& operator=(STexturePool&&) = delete;
private:
    STexturePool();
    ~STexturePool();
    std::vector<STexture> texturePool_;
};

