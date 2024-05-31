#include "RenderBuffer.h"
#include <malloc.h>

TColor defaultColor(0);

RenderBuffer::RenderBuffer(size_t width, size_t height) :width_(width), height_(height) {
	int size = width * height * 4;
	colorBuffer = (unsigned char*)malloc(size * sizeof(size_t));
	clear(defaultColor);
}

RenderBuffer::~RenderBuffer()
{
	free(colorBuffer);
}

void RenderBuffer::setColor(size_t x, size_t y, TColor color)
{
	size_t idx = (y * width_ + x) * 4;
	for (int i = 0; i < 4; i++) {
		colorBuffer[idx + i] = color[i];
	}
}

TColor RenderBuffer::getColor(size_t x, size_t y) {
	size_t idx = (y * width_ + x) * 4;
	return TColor(colorBuffer[idx], colorBuffer[idx + 1], colorBuffer[idx + 2], colorBuffer[idx + 3]);
}

void RenderBuffer::clear(TColor color) {
	int pixelSize = width_ * height_;
	for (int i = 0; i < pixelSize; i++) {
		for (int j = 0; j < 3; j++) {
			colorBuffer[i * 4 + j] = color[j];
		}
	}
}