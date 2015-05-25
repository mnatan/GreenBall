#ifndef EMPTYSPACE_CPP
#define EMPTYSPACE_CPP

#include "EmptySpace.h"

EmptySpace::EmptySpace() : game_obj(zero, -1) {}
bool EmptySpace::canEnter(Map & map, Vector3D &zmiana)
{
	return true;
}
bool EmptySpace::canFall()
{
	return true;
}
bool EmptySpace::drawIt()
{
	return false;
}
bool EmptySpace::UpdateAnimation(double ratio)
{
	return false;
}

#endif
