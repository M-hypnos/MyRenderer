#include "TColor.h"
#include <cassert>
#include <algorithm>

int MAX_NUM = 255;

TColor::TColor() :r(0), g(0), b(0), a(MAX_NUM) {};

TColor::TColor(int C, int A) :r(C), g(C), b(C), a(A) {};

TColor::TColor(int R, int G, int B, int A) :r(R), g(G), b(B), a(A) {}

int& TColor::operator[](const int i) {
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

int TColor::operator[](const int i) const {
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
		self[i] = std::min(self[i] + rhs[i], MAX_NUM);
	}
	return self;
}

TColor TColor::operator*(float intensity) const {
	intensity = std::max(0.f, intensity);
	intensity = std::min(1.f, intensity);
	TColor self = *this;
	for (int i = 0; i < 3; i++) {
		self[i] = self[i] * intensity;
	}
	return self;
}