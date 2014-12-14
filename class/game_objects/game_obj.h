#ifndef GAME_OBJ
#define GAME_OBJ

#include "Vector3D.cpp"
#include "../../build/define.h"
#include "../engine/MapChunk.cpp"

class game_obj
{
	public:
	Vector3D pos;

	game_obj(Vector3D pos_, int TEX_);
	virtual ~game_obj(){}
	unsigned int TEX;

	virtual bool canFall();
	virtual bool canEnter(Vector3D &zmiana);
	virtual bool isSolid();
	virtual bool isAnimated();
	virtual bool drawIt();
	virtual std::string typKlasy(){return "game_obj";};
	virtual void playerEnters(Vector3D &zmiana);
};

#endif
