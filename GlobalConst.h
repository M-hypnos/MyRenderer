#ifndef __GLOBAL_CONST_
#define __GLOBAL_CONST_
#include "core/TColor.h"

enum class RendererMode {
	Point,
	Line,
	Triangle,
};

enum class MoveDir {
	Bottom,
	Top,
	Left,
	Right,
	Forward,
	Back,
};

constexpr int threadNum = 12;

#endif // !__GLOBAL_CONST_
