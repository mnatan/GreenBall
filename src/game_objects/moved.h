#ifndef MOVED
#define MOVED

#include "animated.cpp"
#include "src/engine/Map.h"

class moved: public animated
{
	public:
	Vector3D startPos;
	Vector3D endPos;
	float startTime;
	float endTime;

	moved(Vector3D pos, int TEX): animated(pos, TEX), startPos(pos), endPos(pos), startTime(0), endTime(0){};
	void setAnimation(Vector3D start, Vector3D end, double startTime, double duration);
	bool checkFloor();
	bool UpdateAnimation(double current_time, double ratio);
	virtual std::string typKlasy() { return "moved"; };
};

#endif
