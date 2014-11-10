#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "player.h"
#include "Vector3D.h"

Player::Player()
{
	TEX = TEX_PLAYER;
}
bool Player::pushObj(moved &obj)
{
	Vector3D delta(
	    obj.pos.x - pos.x,
	    obj.pos.y - pos.y,
	    obj.pos.z - pos.z
	);
	Vector3D newspace = obj.pos + delta;
	if (map[(int)newspace.x][(int)newspace.y] != MAP_WALL)
	{
		obj.setAnimation(
		    obj.pos,
		    newspace,
		    ANIM_PLAYER_TIME
		);
		this->setAnimation(
		    this->pos,
		    obj.pos,
		    ANIM_PLAYER_TIME
		);
	}
	return true;
}

#endif
