#include "SColor.h"

SColor::SColor() :r(0), g(0), b(0), a(255)
{}

SColor::SColor(unsigned char c, unsigned char a)
	: r(c), g(c), b(c), a(a)
{}

SColor::SColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
: r(r), g(g), b(b), a(a) 
{}

unsigned char& SColor::operator[](const int i) {
	return i <= 0 ? r : (1 == i ? g : (2 == i ? b : a));
}

unsigned char SColor::operator[](const int i) const {
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