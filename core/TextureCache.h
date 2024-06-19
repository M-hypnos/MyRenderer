#ifndef __TEXTURE_CACHE_H__
#define __TEXTURE_CACHE_H__

#ifdef _EXPORTING  
#define TEXTURE_CACHE_EXPORT    __declspec(dllexport)  
#else  
#define TEXTURE_CACHE_EXPORT    __declspec(dllimport)  
#endif  

#include "Texture.h"
#include <vector>
#include <string>

class TEXTURE_CACHE_EXPORT TextureCache {
public:
    static TextureCache& getInstance();
    int getCacheSize();
    Texture getTexture(int idx);
    int pushToCache(Texture tex);
    int isTextureLoaded(std::string path);

    TextureCache(const TextureCache&) = delete;
    TextureCache(TextureCache&&) = delete;
    TextureCache& operator=(const TextureCache&) = delete;
    TextureCache& operator=(TextureCache&&) = delete;
private:
    TextureCache();
    ~TextureCache();
    std::vector<Texture> texturePool;
};

#endif