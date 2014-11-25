#ifndef myVector
#define myVector

#include <iostream>

class Vector3D
{
	public:
	float x;
	float y;
	float z;

	Vector3D(): x(0), y(0), z(0) {}
	Vector3D(float x_ , float y_, float z_);
	friend Vector3D operator+(const Vector3D &lhs, const Vector3D &rhs);
	friend Vector3D operator-(const Vector3D &lhs, const float rhs);
	friend Vector3D operator*(const Vector3D &lhs, const float rhs);
	friend bool operator==(const Vector3D &lhs, const Vector3D &rhs);
	friend bool operator!=(const Vector3D &lhs, const Vector3D &rhs);
	friend std::ostream& operator<<(std::ostream& os, const Vector3D &rhs);
};
	static const Vector3D zero(0, 0, 0);
	static const Vector3D left(-1, 0, 0);
	static const Vector3D right(1, 0, 0);
	static const Vector3D top(0, -1, 0);
	static const Vector3D bot(0, 1, 0);
	static const Vector3D down(0, 0, -1);
	static const Vector3D up(0, 0, 1);
	static const Vector3D none(0, 0, 0);
#endif
