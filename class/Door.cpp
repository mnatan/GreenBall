#ifndef DOOR_CPP
#define DOOR_CPP

#include "Door.h"

Door::Door(Vector3D pos_)
{
	TEX = TEX_DOOR;
	pos = pos_;
	closedPos = pos_;
	if (MapRead(pos + LEFT) == MAP_WALL)
	{
		openPos = pos + LEFT + (RIGHT * 0.1);
	}
	else if (MapRead(pos + RIGHT) == MAP_WALL)
	{
		openPos = pos + RIGHT + (LEFT * 0.1);
	}
	else if (MapRead(pos + TOP) == MAP_WALL)
	{
		openPos = pos + TOP + (BOT * 0.1);
	}
	else if (MapRead(pos + BOT) == MAP_WALL)
	{
		openPos = pos + BOT + (TOP * 0.1);
	}
	else {
		openPos = pos + Vector3D(0,0,-0.9);
	}
}

bool Door::respondON()
{
	setAnimation(
	    pos,
	    openPos,
	    ANIM_DOOR_CLOSE
	);
	return true;
}
bool Door::respondOFF()
{
	setAnimation(
	    pos,
	    closedPos,
	    ANIM_DOOR_CLOSE
	);
	return true;
}

#endif
