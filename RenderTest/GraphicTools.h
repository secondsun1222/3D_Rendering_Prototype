#pragma once
#include "Vec3.h"
#include "Algorithm"


#define FRAC(x) (x - (int)x)
#define round(x, l, m) (max(min(x,m),l))
#define RGB_R(color) ((color >> 16) & 0xFF)
#define RGB_G(color) ((color >> 8) & 0xFF)
#define RGB_B(color) (color & 0xFF)

typedef POINT Pixel;
bool SortVec3ByY(Vec3 v1, Vec3 v2);

void BoundRectVec3(const Vec3* v, int size, Vec3& min, Vec3& max);