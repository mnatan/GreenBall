#ifndef MOVED
#define MOVED

#include "animated.h"
#include "../define.cpp"

class moved: public animated
{
	public:
	Vector3D startPos;
	Vector3D endPos;
	float startTime;
	float endTime;

	void setAnimation(Vector3D start, Vector3D end, float timestart, float timeend);
	bool UpdateAnimation();
};

#endif
