#ifndef ANIMATED
#define ANIMATED

#include "game_obj.h"
#include "../define.h"

class animated: public game_obj
{
	public:
	bool animating;

	animated(Vector3D pos, int TEX): game_obj(pos, TEX) {}

	virtual bool UpdateAnimation();
	bool isAnimated();
};

#endif
