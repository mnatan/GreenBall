#ifndef ROTATED_CPP
#define ROTATED_CPP

#include "rotated.h"

rotated::rotated(Vector3D pos_, Vector3D vector, float angle_, float acc)
{
	rotationVector = vector;
	angle = angle_;
	accumulator = acc;
	pos = pos_;
}
bool rotated::UpdateAnimation()
{
	accumulator += angle * ratio;
	return true;
}

#endif
