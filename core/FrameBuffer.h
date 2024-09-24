#include "Color.h"
#include "../utils/Math.h"
#pragma once
class FrameBuffer
{
public:
	FrameBuffer(int w, int h, int sampleCount = 1);
	~FrameBuffer();
	void writeBuffer(int x, int y, Color color);
	void readBuffer(int x, int y, Color& color);
	bool depthCheck(int x, int y, float z);
	void clear(Color color = Color(0));
	int getWidth() { return width_; }
	int getHeight() { return height_; }
	Matrix getViewport();
private:
	int width_, height_;
	unsigned char* buffer;
	float* zBuffer;
};

