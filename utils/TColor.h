#ifndef __TCOLOR_H__
#define __TCOLOR_H__

class TColor {
public:
	size_t r, g, b, a;
	TColor();
	TColor(size_t C, size_t A = 255);
	TColor(size_t R, size_t G, size_t B, size_t A = 255);
	//TColor(float R, float G, float B, float A = 255);

	size_t& operator[](const size_t i);
	size_t operator[](const size_t i) const;
	TColor operator+(const TColor& rhs) const;
	TColor operator*(float intensity) const;
};

#endif