#ifndef SCALED_CPP
#define SCALED_CPP

#include "scaled.h"

void scaled::setAnimation(float start, float end, float timestart, float timeend)
{
	startScale = start;
	endScale = end;

	startTime = timestart;
	endTime = timeend;
}

bool scaled::UpdateAnimation()
{
	if (GreenEngine::current_time >= endTime)
	{
		scale = endScale;
		return false;
	}

	float delta = (GreenEngine::current_time  - startTime) / (endTime - startTime);
	scale = startScale + delta * (endScale - startScale);

	return true;
}

#endif
