#ifndef myVector
#define myVector
class Vector3D
{
	public:
	float x;
	float y;
	float z;

	Vector3D(): x(0), y(0), z(0) {}
	Vector3D(float x_ , float y_, float z_);
	friend Vector3D operator+(const Vector3D &lhs, const Vector3D &rhs);
};
#endif
