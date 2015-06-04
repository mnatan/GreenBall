#ifndef BLNKER_CPP
# define BLNKER_CPP

# include "blinker.h"
# include "scaled.cpp"
# include <iostream>

Blinker::Blinker(Vector3D pozycja, float time_dead_, float time_alive_)
{
	pos = pozycja;
	time_dead = time_dead_;
	time_alive = time_alive_;
	scale = 1.0f;
	state = visible;
	next_switch = GreenEngine::current_time + time_alive;
}

bool Blinker::drawIt()
{
	//UpdateAnimation(); //TODO
	GreenEngine::DrawCubeTexture(pos + Vector3D(0, 0, -0.5f),
				     scale, TEX_FLOOR);
	return true;
}

bool Blinker::UpdateAnimation(double current_time, double ratio)
{
	if (current_time > next_switch) {
		if (state == visible) {
			state = shrinking;
			next_switch =
			    current_time + BLINK_FADE_TIME + time_dead;
			scaled::setAnimation(1, 0, current_time,
					     current_time + BLINK_FADE_TIME);
		} else if (state == hidden) {
			state = growing;
			next_switch =
			    current_time + BLINK_FADE_TIME + time_alive;
			setAnimation(0, 1, current_time,
				     current_time + BLINK_FADE_TIME);
			// TODO Włącz spadanie na tym chunku
			return true;
		}
	} else if (state == shrinking) {
		if (endTime < current_time) {
			state = hidden;
			scale = 0;
			// TODO Wyłącz spadanie na tym chunku
			return false;
		} else {
			scaled::UpdateAnimation(current_time, ratio);
		}
	} else if (state == growing) {
		if (endTime < current_time) {
			state = visible;
			scale = 1;
		} else {
			scaled::UpdateAnimation(current_time,ratio);
		}
	}
	return true;
}

bool Blinker::canEnter(Map & map, Vector3D & zmiana)
{
	return true;
}

bool Blinker::canFall()
{
	if (state == hidden)
		return true;
	else
		return false;
}
#endif
