#include "FrameBuffer.h"
#include <malloc.h>
#include <limits>
#include <mutex>
using namespace std;
mutex mtx;

TColor defaultColor(0);

FrameBuffer::FrameBuffer(int width, int height) :width_(width), height_(height) {
	colorBuffer = (int*)malloc(width * height * 4 * sizeof(int));
	zBuffer = (float*)malloc(width * height * sizeof(float));
	clear(defaultColor);
}

FrameBuffer::~FrameBuffer()
{
	free(colorBuffer);
	free(zBuffer);
}

Matrix FrameBuffer::getViewport() {
	Matrix m = Matrix::identity();
	m[0][0] = width_ / 2;
	m[1][1] = height_ / 2;
	m[0][3] = width_ / 2;
	m[1][3] = height_ / 2;
	return m;
}

void FrameBuffer::setColor(int x, int y, TColor color, float z)
{
	mtx.lock();
	int idx = y * width_ + x;
	if (z > zBuffer[idx]) {
		int cIdx = idx * 4;
		for (int i = 0; i < 4; i++) {
			colorBuffer[cIdx + i] = color[i];
		}
		zBuffer[idx] = z;
	}
	mtx.unlock();
}

TColor FrameBuffer::getColor(int x, int y) {
	int idx = (y * width_ + x) * 4;
	return TColor(colorBuffer[idx], colorBuffer[idx], colorBuffer[idx + 2], colorBuffer[idx + 3]);
}

void FrameBuffer::clear(TColor color) {
	int pixelSize = width_ * height_;
	for (int i = 0; i < pixelSize; i++) {
		int idx = i * 4;
		for (int j = 0; j < 3; j++) {
			colorBuffer[idx + j] = color[j];
		}
		zBuffer[i] = -std::numeric_limits<float>::infinity();
	}
}