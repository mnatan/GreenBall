#ifndef WALL_CPP
#define WALL_CPP

#include "Wall.h"

Wall::Wall(Vector3D pos) : game_obj(pos, TEX_WALL)
{
}
bool Wall::canEnter(Map & map, Vector3D &zmiana)
{
	return false;
}
bool Wall::isSolid()
{
	return true;
}
bool Wall::drawIt()
{
    GreenEngine::DrawCubeTexture(
	    pos + Vector3D(0, 0, 0.5f),
	    1.0f,
	    TEX
	);
	return true;
}

#endif
