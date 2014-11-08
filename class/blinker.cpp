#ifndef BLNKER_CPP
#define BLNKER_CPP

#include "blinker.h"
#include "scaled.cpp"
#include <iostream>

Blinker::Blinker(Vector3D pozycja, float time_dead_, float time_alive_)
{
	pos = pozycja;
	time_dead = time_dead_;
	time_alive = time_alive_;
	scale = 1.0f;
	state = visible;
	next_switch = current_time + time_alive;
}
bool Blinker::UpdateAnimation()
{
	if (current_time > next_switch)
	{
		if (state == visible)
		{
			state = shrinking;
			next_switch =  current_time + BLINK_FADE_TIME + time_dead;
			scaled::setAnimation(
			    1, 0,
			    current_time,
			    current_time + BLINK_FADE_TIME
			);
		}
		else if (state == hidden)
		{
			state = growing;
			next_switch =  current_time + BLINK_FADE_TIME + time_alive;
			setAnimation(
			    0, 1,
			    current_time,
			    current_time + BLINK_FADE_TIME
			);
			map[(int)pos.x][(int)pos.y] = MAP_BLINKER;
		}
	}
	else if (state == shrinking)
	{
		if (endTime < current_time)
		{
			state = hidden;
			scale = 0;
			map[(int)pos.x][(int)pos.y] = MAP_NONE;
		}
		else
		{
			scaled::UpdateAnimation();
		}
	}
	else if (state == growing)
	{
		if (endTime < current_time)
		{
			state = visible;
			scale = 1;
		}
		else
		{
			scaled::UpdateAnimation();
		}
	}
	return true;
}
#endif
