#include "SColor.h"
#include "../utils/Math.h"
#pragma once
class SFrameBuffer
{
public:
	SFrameBuffer(int w, int h, int sampleCount = 1);
	~SFrameBuffer();
	void writeBuffer(int x, int y, SColor color);
	void readBuffer(int x, int y, SColor& color);
	bool depthCheck(int x, int y, float z);
	void clear(SColor color = SColor(0));
	int getWidth() { return width_; }
	int getHeight() { return height_; }
	Matrix getViewport();
private:
	int width_, height_;
	unsigned char* buffer;
	float* zBuffer;
};

