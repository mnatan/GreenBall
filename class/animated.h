#ifndef ANIMATED
#define ANIMATED

#include "game_obj.h"
#include "../define.h"

class animated: public game_obj
{
	public:
	bool active;

	bool UpdateAnimation();
};

#endif
