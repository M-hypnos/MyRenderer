#pragma once
class Color
{
public:
public:
	Color();
	Color(unsigned char c, unsigned char a = 255);
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	unsigned char& operator[](const int i);
	unsigned char operator[](const int i) const;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

