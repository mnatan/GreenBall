#ifndef EMPTYSPACE
#define EMPTYSPACE

#include "game_obj.cpp"
#include "../define.h"

class EmptySpace: public game_obj
{
	public:
	EmptySpace();

	bool canEnter();
	bool canFall();
	bool UpdateAnimation();
	bool drawIt();
};

#endif
