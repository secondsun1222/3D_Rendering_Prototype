#pragma once
#include "pch.h"

struct Vec3 {
public:
	double x, y, z;

public:
	Vec3();
	Vec3(double _x, double _y);
	Vec3(double _x, double _y, double _z);
	Vec3(int _x, int _y);
	Vec3(int _x, int _y, int _z);

	//normal operators
	Vec3 operator+(const Vec3& v2) const;
	Vec3 operator-(const Vec3& v2) const;
	Vec3 operator*(const double& d) const;
	Vec3 operator/(const double& d) const;
	Vec3 operator+=(const Vec3& v2);
	Vec3 operator-=(const Vec3& v2);
	Vec3 operator*=(const double& d);
	Vec3 operator/=(const double& d);

	Vec3 operator-() const;

	bool operator==(const Vec3& v2) const;

	//vector properties
	double magnitude();
	Vec3 unit();

	//vector operators
	static double dot(const Vec3& v1, const Vec3& v2);
	static Vec3 cross(const Vec3& v1, const Vec3& v2);
};