#ifndef MOVED_CPP
# define MOVED_CPP

# include "moved.h"

void moved::setAnimation(Vector3D start, Vector3D end, double duration)
{
	animating = true;
	startPos = start;
	endPos = end;
	startTime = GreenEngine::current_time;
	endTime = GreenEngine::current_time + duration;
}

bool moved::UpdateAnimation()
{
	if (GreenEngine::current_time >= endTime) {
		pos = endPos;
		//checkFloor();
		return false;
	}

	float delta = (GreenEngine::current_time - startTime) / (endTime - startTime);

	pos = Vector3D(startPos.x + delta * (endPos.x - startPos.x),
		       startPos.y + delta * (endPos.y - startPos.y),
		       startPos.z + delta * (endPos.z - startPos.z)
	    );

	return true;
}

bool moved::checkFloor()
{
    /*
	 *if (map.access(pos + down).canFall())	// Possible bug FIXME - Czy ja zawsze będę trzymać dane w ten sposób? //AND YES IT HAPPENED
	 *{
	 *    this->setAnimation(pos,
	 *               Vector3D(pos.x, pos.y, -22.0f),
	 *               ANIM_TIME_FALL);
	 *    return true;
	 *}
     */
	return false;
}

#endif
