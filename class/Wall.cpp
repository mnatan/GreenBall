#ifndef WALL_CPP
#define WALL_CPP

#include "Wall.h"

Wall::Wall(Vector3D pos) : game_obj(pos, TEX_WALL)
{
}
bool Wall::canEnter()
{
	return false;
}
bool Wall::isSolid()
{
	return true;
}
bool Wall::drawIt()
{
	DrawCubeTexture(
	    pos + Vector3D(0, 0, 0.5f),
	    1.0f,
	    TEX
	);
	DrawCubeTexture(
	    pos + Vector3D(0, 0, -0.5f),
	    1.0f,
	    TEX_FLOOR
	);
	return true;
}

#endif
