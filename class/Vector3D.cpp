#ifndef myVector_CPP
#define myVector_CPP

#include "Vector3D.h"

Vector3D::Vector3D(float x_ , float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

Vector3D operator+(const Vector3D &lhs, const Vector3D &rhs)
{
	return Vector3D(
	           lhs.x + rhs.x,
	           lhs.y + rhs.y,
	           lhs.z + rhs.z
	       );
}

std::ostream& operator<<(std::ostream& os, const Vector3D &rhs)
{
	os << "(" << rhs.x << ","<< rhs.y << "," << rhs.z << ")";
	return os;
}

#endif
