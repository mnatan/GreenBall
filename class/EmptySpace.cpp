#ifndef EMPTYSPACE_CPP
#define EMPTYSPACE_CPP

#include "EmptySpace.h"

EmptySpace::EmptySpace() : game_obj(zero, -1) {}
bool EmptySpace::canEnter()
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
bool EmptySpace::UpdateAnimation()
{
	return false;
}

#endif
