#ifndef MOVED
#define MOVED

#include "animated.cpp"
#include "class/engine/Map.h"

class moved: public animated
{
	public:
	Vector3D startPos;
	Vector3D endPos;
	float startTime;
	float endTime;

	moved(Vector3D pos, int TEX): animated(pos, TEX), startPos(pos), endPos(pos), startTime(0), endTime(0){};
	void setAnimation(Vector3D start, Vector3D end, float duration);
	bool checkFloor(Map & map);
	bool UpdateAnimation();
	virtual std::string typKlasy() { return "moved"; };
};

#endif
