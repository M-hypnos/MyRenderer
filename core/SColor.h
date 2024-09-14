#pragma once
class SColor
{
public:
public:
	SColor();
	SColor(unsigned char c, unsigned char a = 255);
	SColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	unsigned char& operator[](const int i);
	unsigned char operator[](const int i) const;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

