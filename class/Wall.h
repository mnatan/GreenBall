#ifndef WALL
#define WALL

#include "game_obj.cpp"

class Wall: public game_obj
{
	bool canEnter();
};

#endif
