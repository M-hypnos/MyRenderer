#include "Button.h"
#include "../core/Pipeline.h"

Button::Button(int x, int y, int width, int height, std::function <void()> callback, const char* str, SDL_Renderer* renderer)
:x_(x), y_(y), width_(width), height_(height), onCallback_(callback) {
	font = TTF_OpenFont("res/font1.ttf", 50);
	if (font == NULL)
		return;

	SDL_Surface* message = TTF_RenderText_Blended(font, str, {255, 255, 255});
	pTextTexture_ = SDL_CreateTextureFromSurface(renderer, message);
	SDL_FreeSurface(message);
}

Button::~Button() {
	SDL_DestroyTexture(pTextTexture_);
}

void Button::onPressDown(int x, int y) {
	if (x >= x_ && x <= x_ + width_ && y >= y_ && y <= y_ + height_) {
		isPress_ = true;
	}
}

void Button::onRender(SDL_Renderer* renderer) {
	if (isPress_) 
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	else
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect r;
	r.x = x_;
	r.y = y_;
	r.w = width_;
	r.h = height_;
	SDL_RenderDrawRect(renderer, &r);

	SDL_RenderCopy(renderer, pTextTexture_, NULL, &r);
}