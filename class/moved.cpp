#ifndef MOVED_CPP
#define MOVED_CPP

#include "moved.h"
float current_time;

void moved::setAnimation(Vector3D start, Vector3D end, float timestart, float timeend)
{
	startPos = start;
	endPos = end;
	startTime = timestart;
	endTime = timeend;
}
bool moved::UpdateAnimation()
{
	if (current_time >= endTime)
	{
		pos = endPos;
		return false;
	}

	float delta = (current_time  - startTime) / (endTime - startTime);

	pos = Vector3D(
	          startPos.x + delta * (endPos.x - startPos.x),
	          startPos.y + delta * (endPos.y - startPos.y),
	          startPos.z + delta * (endPos.z - startPos.z)
	      );

	return true;
}

#endif
