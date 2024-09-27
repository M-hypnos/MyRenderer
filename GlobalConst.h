#pragma once
enum class RendererMode {
	Point,
	Line,
	Triangle,
	Normal,
};

enum class MoveDir {
	Bottom,
	Top,
	Left,
	Right,
	Forward,
	Back,
};

enum class Plane {
	XLeft,
	XRight,
	YTOP,
	YBOTTOM,
	ZNEAR,
	ZFAR,
};