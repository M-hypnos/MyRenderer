#ifndef __FREAME_BUFFER_H__
#define __FREAME_BUFFER_H__

#include "TColor.h"
#include "../utils/Geometry.h"

class FrameBuffer {
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();
	Matrix getViewport();
	void setColor(int x, int y, TColor color, float z = 0.f);
	TColor getColor(int x, int y);
	void clear(TColor color);
	int getWidth() { return width_; }
	int getHeight() { return height_; }
private:
	int width_, height_;
	int* colorBuffer;
	float* zBuffer;
};

#endif