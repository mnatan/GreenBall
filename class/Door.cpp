#ifndef DOOR_CPP
#define DOOR_CPP

#include "Door.h"

Door::Door(Vector3D pos_) : moved(pos_, TEX_DOOR)
{
	closedPos = pos_;
	if (MapRead(pos + left).statyczny->isSolid())
	{
		openPos = pos + left + (right * 0.1);
	}
	else if (MapRead(pos + right).statyczny->isSolid())
	{
		openPos = pos + right + (left * 0.1);
	}
	else if (MapRead(pos + top).statyczny->isSolid())
	{
		openPos = pos + top + (bot * 0.1);
	}
	else if (MapRead(pos + bot).statyczny->isSolid())
	{
		openPos = pos + bot + (top * 0.1);
	}
	else
	{
		openPos = pos + Vector3D(0, 0, -0.9);
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
bool Door::drawIt()
{
	UpdateAnimation();
	DrawCubeTexture(
	    pos + Vector3D(0, 0, 0.7f),
	    0.95f,
	    TEX
	);
	return true;
}

bool Door::canEnter()
{
	if (pos == openPos) return true;
	else return false;
}

#endif
