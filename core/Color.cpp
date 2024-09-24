#include "Color.h"

Color::Color() :r(0), g(0), b(0), a(255)
{}

Color::Color(unsigned char c, unsigned char a)
	: r(c), g(c), b(c), a(a)
{}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
: r(r), g(g), b(b), a(a) 
{}

unsigned char& Color::operator[](const int i) {
	return i <= 0 ? r : (1 == i ? g : (2 == i ? b : a));
}

unsigned char Color::operator[](const int i) const {
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