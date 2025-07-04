#include "Vec2.h"

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(double _x, double _y) : x(_x), y(_y) {}
Vec2::Vec2(int _x, int _y) : x((double)_x), y((double)_y) {}

Vec2 Vec2::operator+(const Vec2& v2) const
{
	return Vec2(x + v2.x, y + v2.y);
}
Vec2 Vec2::operator-(const Vec2& v2) const
{
	return Vec2(x - v2.x, y - v2.y);
}
Vec2 Vec2::operator*(const double& d) const
{
	return Vec2(x * d, y * d);
}
Vec2 Vec2::operator/(const double& d) const
{
	return Vec2(x / d, y / d);
}