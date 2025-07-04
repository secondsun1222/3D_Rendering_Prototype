#include "GraphicTools.h"

bool SortVec3ByY(Vec3 v1, Vec3 v2) {
	return v1.y < v2.y ? true : false;
}

void BoundRectVec3(const Vec3* v, int size, Vec3& min, Vec3& max) {
	if (size <= 0) return;

	min.x = v[0].x;
	min.y = v[0].y;
	max.x = v[0].x;
	max.y = v[0].y;

	for (int i = 1; i < size; i++) {
		if (v[i].x < min.x)min.x = v[i].x;
		if (v[i].y < min.y)min.y = v[i].y;
		if (max.x < v[i].x)max.x = v[i].x;
		if (max.y < v[i].y)max.y = v[i].y;
	}
}