#pragma once
#include "pch.h"

struct Vec2 {
public:
	double x, y;

public:
	Vec2();
	Vec2(double _x, double _y);
	Vec2(int _x, int _y);

	//normal operators
	Vec2 operator+(const Vec2& v2) const;
	Vec2 operator-(const Vec2& v2) const;
	Vec2 operator*(const double& d) const;
	Vec2 operator/(const double& d) const;
};
