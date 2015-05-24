#ifndef GAMEOBJ_CPP
#define GAMEOBJ_CPP

#include "game_obj.h"

game_obj::game_obj(Vector3D pos_, int TEX_) : pos(pos_), TEX(TEX_) {}

bool game_obj::canFall()
{
	return false;
}
bool game_obj::canEnter(Map & map, Vector3D &zmiana)
{
	return true;
}
bool game_obj::isSolid()
{
	return false;
}
bool game_obj::isAnimated()
{
	return false;
}
bool game_obj::drawIt()
{
	return false;
}
void game_obj::playerEnters(Map & map, Vector3D &zmiana){ return; }

#endif
