#ifndef ROTATED_CPP
#define ROTATED_CPP

#include "rotated.h"

rotated::rotated(Vector3D pos_, Vector3D vector, float angle_, float acc) : animated(pos_, TEX_GEM), rotationVector(vector), angle(angle_), accumulator(acc){ }

bool rotated::UpdateAnimation(double ratio)
{
	accumulator += angle * ratio;
	return true;
}

#endif
