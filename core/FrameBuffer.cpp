#include "FrameBuffer.h"
#include <malloc.h>
#include <limits>

FrameBuffer::FrameBuffer(int w, int h, int sampleCount): width_(w), height_(h){
	buffer = (unsigned char*)malloc(width_ * height_ * 4 * sizeof(unsigned char));
	zBuffer = (float*)malloc(width_ * height_ * sizeof(float));
	clear();
}

FrameBuffer::~FrameBuffer() {
	if(buffer != nullptr)
		free(buffer);
	if (zBuffer != nullptr)
		free(zBuffer);
}

bool FrameBuffer::depthCheck(int x, int y, float z) {
	size_t idx = (y * width_ + x);
	if (z < zBuffer[idx]) return false;
	zBuffer[idx] = z;
	return true;
}

void FrameBuffer::writeBuffer(int x, int y, Color color)
{
	size_t idx = (y * width_ + x) * 4;
	buffer[idx] = color[0];
	buffer[idx+1] = color[1];
	buffer[idx+2] = color[2];
	buffer[idx+3] = color[3];
}

void FrameBuffer::readBuffer(int x, int y, Color& c) {
	size_t idx = (y * width_ + x) * 4;
	c.r = buffer[idx];
	c.g = buffer[idx+1];
	c.b = buffer[idx+2];
	c.a = buffer[idx+3];
}

void FrameBuffer::clear(Color color) {
	size_t pixelSize = width_ * height_;
	for (size_t i = 0; i < pixelSize; i++) {
		size_t idx = i * 4;
		buffer[idx] = color[0];
		buffer[idx+1] = color[1];
		buffer[idx+2] = color[2];
		buffer[idx+3] = color[3];
		zBuffer[i] = -std::numeric_limits<float>::infinity();
	}
}

Matrix FrameBuffer::getViewport() {
	Matrix m = Matrix::identity();
	m[0][0] = width_ / 2;
	m[1][1] = height_ / 2;
	m[0][3] = width_ / 2;
	m[1][3] = height_ / 2;
	return m;
}