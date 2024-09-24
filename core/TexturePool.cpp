#include "TexturePool.h"
#include <iostream>
TexturePool& TexturePool::getInstance() {
	static TexturePool instance;
	return instance;
}

TexturePool::TexturePool() {

}

TexturePool::~TexturePool() {

}

Texture TexturePool::getTexture(int idx) {
	return texturePool_[idx];
}

Texture TexturePool::getTexture(const char* filename) {
	for (int i = 0; i < texturePool_.size(); i++) {
		if (strcmp(texturePool_[i].filename, filename) == 0) {
			return texturePool_[i];
		}
	}

	Texture texture;
	if (texture.loadFromImage(filename, "")) {
		texturePool_.push_back(texture);
	}
	return texturePool_[texturePool_.size() - 1];
}

int TexturePool::pushToCache(Texture tex) {
	texturePool_.push_back(tex);
	return texturePool_.size() - 1;
}
int TexturePool::isTextureLoaded(const char* filename) {
	for (int i = 0; i < texturePool_.size(); i++) {
		if (strcmp(texturePool_[i].filename, filename) == 0) {
			return i;
		}
	}
	return -1;
}