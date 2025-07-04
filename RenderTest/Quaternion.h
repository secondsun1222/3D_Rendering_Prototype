#pragma once
#include "pch.h"
#include "Vec3.h"

struct Quaternion
{
	double w;
	Vec3 v;
public:
	Quaternion(); // default quaternion, pointing +x (probably)
	Quaternion(double _w, Vec3 _v);

	Quaternion inverse() const; // inverse of a quaternion, to multiply from the right

	static Quaternion multiply(const Quaternion& q, const Quaternion& apply);
	static Vec3 rotate(const Quaternion& rotation, const Vec3& point);
};

Quaternion makeQuaternion(const double& theta, const Vec3& axis);
// returns a quaternion that rotates theta around axis
// axis should be normalized to work as a normal rotation

Quaternion eulerToQuaternion(const Vec3& euler);
// euler angle -> quaternion
// origin : Vec3(1,0,0)