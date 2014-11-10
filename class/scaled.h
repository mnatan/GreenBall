#ifndef SCALED
#define SCALED

#include "animated.h"
#include "../define.h"

class scaled: public animated
{
	public:
	float scale;

	float startScale;
	float endScale;

	float startTime; //start time
	float endTime; //start time

	void setAnimation(float start, float end, float timestart, float timeend);
	bool UpdateAnimation();
};

#endif
