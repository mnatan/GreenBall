#ifndef WALL
#define WALL

#include "game_obj.cpp"
#include "../define.h"

class Wall: public game_obj
{
	public:
	Wall(Vector3D pos);

	bool canEnter();
	bool isSolid();
	bool drawIt();
	virtual std::string typKlasy(){ return "Wall";};
};

#endif
