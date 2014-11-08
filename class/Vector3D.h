#ifndef myVector
#define myVector
class Vector3D
{
	public:
	float x;
	float y;
	float z;

	Vector3D(): x(0), y(0), z(0) {}
	Vector3D(float x_ , float y_, float z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}
};
#endif
