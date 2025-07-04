#include "Quaternion.h"

/// How to rotate a Vec3 or a Quaternion
/// 
/// Rotating Vec3
/// 1. Make the desired rotation Quaternion using the makeQuaternion() method.
/// 2. Rotate the Vec3 using the Quaternion::rotate() method. The return value is the rotated position.
/// 
/// Rotation Quaternion
/// 1. Make the desired rotation Quaternion using the makeQuaternion() method.
/// 2. Rotate the Quaternion using the Quaternion::multiply() method. The return value is the rotated position.
/// 
/// How to rotate a Vec3 or a Quaternion

Quaternion::Quaternion() : w(0), v{ 0,0,1 } {}
Quaternion::Quaternion(double _w, Vec3 _v) : w(_w), v(_v) {}

Quaternion Quaternion::inverse() const
{
	return Quaternion(w, -v);
}

Quaternion Quaternion::multiply(const Quaternion& q, const Quaternion& apply)
{
	return Quaternion(
		q.w * apply.w - Vec3::dot(q.v, apply.v),
		q.v * apply.w + apply.v * q.w + Vec3::cross(q.v, apply.v)
	);
}

Vec3 Quaternion::rotate(const Quaternion& q, const Vec3& point)
{
	return Quaternion::multiply(
		Quaternion::multiply(q, Quaternion(0, point)),
		q.inverse()
	).v;
}

Quaternion makeQuaternion(const double& theta, const Vec3& axis)
{
	return Quaternion(cos(theta / 2), axis * sin(theta / 2));
}


Quaternion eulerToQuaternion(const Vec3& euler)
{
	double cx = cos(euler.x / 2);
	double sx = sin(euler.x / 2);
	double cy = cos(euler.y / 2);
	double sy = sin(euler.y / 2);
	double cz = cos(euler.z / 2);
	double sz = sin(euler.z / 2);

	return Quaternion(
		cx * cy * cz + sx * sy * sz,
		Vec3(
			sx * cy * cz - cx * sy * sz,
			cx * sy * cz + sx * cy * sz,
			cx * cy * sz - sx * sy * cz
		)
	);
}