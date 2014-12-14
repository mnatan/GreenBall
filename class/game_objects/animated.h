#ifndef ANIMATED
#define ANIMATED

#include "game_obj.cpp"

class animated: public game_obj
{
	public:
	bool animating;

	animated(Vector3D pos, int TEX): game_obj(pos, TEX) {}

	virtual bool UpdateAnimation();
	bool isAnimated();
	virtual std::string typKlasy() { return "animated"; };
};

#endif
