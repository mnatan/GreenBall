#ifndef MOVED
#define MOVED

#include "animated.cpp"
#include "../define.h"

class moved: public animated
{
	public:
	Vector3D startPos;
	Vector3D endPos;
	float startTime;
	float endTime;

	moved(): startPos(pos), endPos(pos), startTime(0), endTime(0){}
	void setAnimation(Vector3D start, Vector3D end, float duration);
	bool checkFloor();
	bool UpdateAnimation();
};

#endif
