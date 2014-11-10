#ifndef DOOR_CPP
#define DOOR_CPP

#include "Door.h"

Door::Door(Vector3D pos_)
{
	pos = pos_;
	closedPos = pos_;
	if (MapRead(pos + LEFT) == MAP_WALL)
	{
		openPos = pos + LEFT;
	}
	else if (MapRead(pos + RIGHT) == MAP_WALL)
	{
		openPos = pos + RIGHT;
	}
	else if (MapRead(pos + TOP) == MAP_WALL)
	{
		openPos = pos + TOP;
	}
	else if (MapRead(pos + BOT) == MAP_WALL)
	{
		openPos = pos + BOT;
	}
}

bool Door::respondON()
{
	std::cout << "wzywaja mnie!" << std::endl;
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
