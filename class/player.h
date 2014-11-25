#ifndef PLAYER
#define PLAYER

#include "moved.cpp"
#include "../define.h"

class Player: public moved
{
	public:
	Player() : moved(zero, TEX_PLAYER){};
	bool pushObj(moved &obj);
};

#endif
