#ifndef PLAYER
#define PLAYER

#include "moved.cpp"

class Player: public moved
{
	public:
	Player() : moved(zero, TEX_PLAYER){};
	bool pushObj(moved &obj);
};

#endif
