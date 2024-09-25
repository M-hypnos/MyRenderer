#pragma once
#include <functional>
#include <SDL_ttf.h>
class Button
{
public:
	Button(int x, int y, int width, int height, std::function <void()> callback, const char* str, SDL_Renderer* renderer);
	~Button();
	void onPressDown(int clickX, int clickY);
	void onPressUp() {
		if (isPress_) {
			isPress_ = false;
			onCallback_();
		}
	}
	void onRender(SDL_Renderer* renderer);
private:
	int x_;
	int y_;
	int width_;
	int height_;
	std::function <void()> onCallback_;
	bool isPress_;
	SDL_Texture* pTextTexture_;

	TTF_Font* font = nullptr;
};

