#ifndef PLAYER
#define PLAYER

#include "moved.cpp"
#include "../define.h"

class Player: public moved
{
	public:
	Player();
	bool pushObj(moved &obj);
};

#endif
