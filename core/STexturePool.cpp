#include "STexturePool.h"
#include <iostream>
STexturePool& STexturePool::getInstance() {
	static STexturePool instance;
	return instance;
}

STexturePool::STexturePool() {

}

STexturePool::~STexturePool() {

}

STexture STexturePool::getTexture(int idx) {
	return texturePool_[idx];
}

STexture STexturePool::getTexture(const char* filename) {
	for (int i = 0; i < texturePool_.size(); i++) {
		if (strcmp(texturePool_[i].filename, filename) == 0) {
			return texturePool_[i];
		}
	}

	STexture texture;
	if (texture.loadFromImage(filename, "")) {
		texturePool_.push_back(texture);
	}
	return texturePool_[texturePool_.size() - 1];
}

int STexturePool::pushToCache(STexture tex) {
	texturePool_.push_back(tex);
	return texturePool_.size() - 1;
}
int STexturePool::isTextureLoaded(const char* filename) {
	for (int i = 0; i < texturePool_.size(); i++) {
		if (strcmp(texturePool_[i].filename, filename) == 0) {
			return i;
		}
	}
	return -1;
}