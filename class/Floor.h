#ifndef FLOOR
#define FLOOR

#include "../define.h"
#include "game_obj.cpp"

class Floor: public game_obj
{
	public:
	bool stable;
	Floor(Vector3D pos_);

	bool canEnter();
	bool drawIt();
};

#endif
