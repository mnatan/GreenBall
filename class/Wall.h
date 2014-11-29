#ifndef WALL
#define WALL

#include "game_obj.cpp"
#include "../define.h"

class Wall: public game_obj
{
	public:
	Wall(Vector3D pos);

	bool canEnter(Vector3D &zmiana);
	bool isSolid();
	bool drawIt();
	virtual std::string typKlasy(){ return "Wall";};
};

#endif
