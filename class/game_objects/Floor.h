#ifndef FLOOR
#define FLOOR

#include "class/game_objects/game_obj.cpp"

class Floor: public game_obj
{
	public:
	bool stable;
	Floor(Vector3D pos_);

	bool canEnter(Vector3D &zmiana);
	bool drawIt();
	virtual std::string typKlasy(){ return "Floor";};
};

#endif
