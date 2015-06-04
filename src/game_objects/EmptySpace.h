#ifndef EMPTYSPACE
#define EMPTYSPACE

#include "game_obj.cpp"

class EmptySpace: public game_obj
{
	public:
	EmptySpace();

	bool canEnter(Map & map, Vector3D &zmiana);
	bool canFall();
	bool UpdateAnimation(double current_time, double ratio);
	bool drawIt();
	virtual std::string typKlasy(){ return "EmptySpace";};
};

#endif