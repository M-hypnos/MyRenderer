#ifndef __RENDER_BUFFER_H__
#define __RENDER_BUFFER_H__

#include "../utils/TColor.h"

class RenderBuffer {
public:
	RenderBuffer(size_t width, size_t height);
	~RenderBuffer();
	void setColor(size_t x, size_t y, TColor color);
	TColor getColor(size_t x, size_t y);
	void clear(TColor color);
private:
	size_t width_, height_;
	unsigned char* colorBuffer;
};

#endif