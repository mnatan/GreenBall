#ifndef EMPTYSPACE
#define EMPTYSPACE

#include "game_obj.cpp"

class EmptySpace: public game_obj
{
	public:
	EmptySpace();

	bool canEnter(Vector3D &zmiana);
	bool canFall();
	bool UpdateAnimation();
	bool drawIt();

	virtual std::string typKlasy(){ return "EmptySpace";};
};

#endif
