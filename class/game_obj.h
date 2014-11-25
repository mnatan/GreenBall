#ifndef GAME_OBJ
#define GAME_OBJ

#include "Vector3D.cpp"

class game_obj
{
	public:
	Vector3D pos;

	game_obj(Vector3D pos_, int TEX_);
	virtual ~game_obj(){}
	unsigned int TEX;

	virtual bool canFall();
	virtual bool canEnter();
	virtual bool isSolid();
	virtual bool isAnimated();
	virtual bool drawIt();
};

#endif
