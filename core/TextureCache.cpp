#include "TextureCache.h"
#include <cassert>

TextureCache& TextureCache::getInstance() {
	static TextureCache instance;
	return instance;
}

TextureCache::TextureCache() {

}

TextureCache::~TextureCache() {

}

int TextureCache::getCacheSize() {
	return texturePool.size();
}

Texture TextureCache::getTexture(int idx) {
	assert(idx >= 0 && idx < texturePool.size());
	return texturePool[idx];
}

int TextureCache::pushToCache(Texture tex) {
	texturePool.push_back(tex);
	return texturePool.size() - 1;
}
int TextureCache::isTextureLoaded(std::string path) {
	for (int i = 0; i < texturePool.size(); i++) {
		if (texturePool[i].path == path) {
			return i;
		}
	}
	return -1;
}