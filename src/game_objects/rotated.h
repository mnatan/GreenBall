#ifndef ROTATED
#define ROTATED

#include "animated.h"

class rotated: public animated
{
	public:
	Vector3D rotationVector;
	float angle;
	float accumulator;

	rotated(Vector3D pos_, Vector3D vector, float angle_, float acc);
	//void setAnimation(Vector3D start, Vector3D end, float timestart, float timeend);
	bool UpdateAnimation(double current_time, double ratio);
};

#endif
