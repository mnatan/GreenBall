#ifndef GAME_OBJ
#define GAME_OBJ

#include "build/define.h"
#include "class/engine/Vector3D.cpp"
#include "class/engine/MapChunk.cpp"
#include "class/engine/Map.cpp"

class game_obj
{
	public:
	Vector3D pos;

	game_obj(Vector3D pos_, int TEX_);
	virtual ~game_obj(){}
	unsigned int TEX;

	virtual bool canFall();
	virtual bool canEnter(Map & map, Vector3D &zmiana);
	virtual void playerEnters(Map & map, Vector3D &zmiana);
	virtual bool isSolid();
	virtual bool isAnimated();
	virtual bool drawIt();
	virtual std::string typKlasy(){return "game_obj";};
};

#endif
