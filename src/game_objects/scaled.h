#ifndef SCALED
#define SCALED

#include "animated.h"

class scaled: public animated
{
	public:
	float scale;

	float startScale;
	float endScale;

	float startTime; //start time
	float endTime; //start time

	scaled() : animated(zero,-1){};

	void setAnimation(float start, float end, float timestart, float timeend);
	bool UpdateAnimation(double ratio);
};

#endif
