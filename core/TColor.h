#ifndef __TCOLOR_H__
#define __TCOLOR_H__

class TColor {
public:
	int r, g, b, a;
	TColor();
	TColor(int C, int A = 255);
	TColor(int R, int G, int B, int A = 255);

	int& operator[](const int i);
	int operator[](const int i) const;
	TColor operator+(const TColor& rhs) const;
	TColor operator*(float intensity) const;
};

#endif