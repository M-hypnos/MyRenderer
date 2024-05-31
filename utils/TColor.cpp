#include "TColor.h"
#include <cassert>
#include <algorithm>

size_t MAX_NUM = 255;

TColor::TColor() :r(0), g(0), b(0), a(MAX_NUM) {};

TColor::TColor(size_t C, size_t A) :r(C), g(C), b(C), a(A) {};

TColor::TColor(size_t R, size_t G, size_t B, size_t A) :r(R), g(G), b(B), a(A) {}

//TColor::TColor(float R, float G, float B, float A) :r(R), g(G), b(B), a(A) {}

size_t& TColor::operator[](const size_t i) {
	assert(i >= 0 && i <= 3);
	switch (i)
	{
	case 0:
		return r;
	case 1:
		return g;
	case 2:
		return b;
	default:
		return a;
	}
}

size_t TColor::operator[](const size_t i) const {
	assert(i >= 0 && i <= 3);
	switch (i)
	{
	case 0:
		return r;
	case 1:
		return g;
	case 2:
		return b;
	default:
		return a;
	}
}

TColor TColor::operator+(const TColor& rhs) const {
	TColor self = *this;
	for (int i = 0; i < 3; i++) {
		self[i] = self[i] + rhs[i];
		if (self[i] > MAX_NUM) self[i] = MAX_NUM;
	}
	return self;
}

TColor TColor::operator*(float intensity) const {
	intensity = std::max(0.f, intensity);
	intensity = std::min(1.f, intensity);
	TColor self = *this;
	for (int i = 0; i < 3; i++) {
		self[i] = self[i] * intensity;
		if (self[i] > MAX_NUM) self[i] = MAX_NUM;
	}
	return self;
}