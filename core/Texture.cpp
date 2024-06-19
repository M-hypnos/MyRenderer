#include "Texture.h"

bool Texture::loadFromImage(std::string Path, std::string Type) {
    path = Path;
    type = Type;

    loadedSurface = IMG_Load(Path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", Path.c_str(), IMG_GetError());
        return false;
    }

    width = loadedSurface->w;
    height = loadedSurface->h;

    return true;
}

TColor Texture::getPixel(float u, float v) {
    int bpp = loadedSurface->format->BytesPerPixel;
    int x = u * width;
    int y = v * height;
    Uint32 pixel = *((Uint32*)loadedSurface->pixels + (loadedSurface->h - y) * loadedSurface->w + x);
    Uint8 r, g, b, a;
    SDL_GetRGBA(pixel, loadedSurface->format, &r, &g, &b, &a);
    return TColor(r, g, b, a);
}