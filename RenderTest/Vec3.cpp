#include "Vec3.h"

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(double _x, double _y) : x(_x), y(_y), z(0) {}
Vec3::Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
Vec3::Vec3(int _x, int _y) : x((double)_x), y((double)_y), z(0) {}
Vec3::Vec3(int _x, int _y, int _z) : x((double)_x), y((double)_y), z((double)_z) {}

Vec3 Vec3::operator+(const Vec3& v2) const
{
	return Vec3(x + v2.x, y + v2.y, z + v2.z);
}
Vec3 Vec3::operator-(const Vec3& v2) const
{
	return Vec3(x - v2.x, y - v2.y, z - v2.z);
}
Vec3 Vec3::operator*(const double& d) const
{
	return Vec3(x * d, y * d, z * d);
}
Vec3 Vec3::operator/(const double& d) const
{
	return Vec3(x / d, y / d, z / d);
}

Vec3 Vec3::operator+=(const Vec3& v2)
{
	x += v2.x;	y += v2.y;	z += v2.z;
	return *this;
}
Vec3 Vec3::operator-=(const Vec3& v2)
{
	x -= v2.x;	y -= v2.y;	z -= v2.z;
	return *this;
}
Vec3 Vec3::operator*=(const double& d)
{
	x *= d;	y *= d;	z *= d;
	return *this;
}
Vec3 Vec3::operator/=(const double& d)
{
	x /= d;	y /= d;	z /= d;
	return *this;
}

Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

bool Vec3::operator==(const Vec3& v2) const
{
	return x == v2.x && y == v2.y && z == v2.z;
}

double Vec3::magnitude() { return sqrt(x * x + y * y + z * z); }

Vec3 Vec3::unit() { return (*this) / this->magnitude(); }

double Vec3::dot(const Vec3& v1, const Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
Vec3 Vec3::cross(const Vec3& v1, const Vec3& v2)
{
	return { v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x };
}