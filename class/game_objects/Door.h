#ifndef DOOR
#define DOOR

#include "Responder.cpp"
#include "game_obj.cpp"

class Door: public moved //, public Responder
{
	public:
	Vector3D openPos;
	Vector3D closedPos;

	Door(Vector3D pos_);
	bool respondON();
	bool respondOFF();

	bool drawIt();

	bool canEnter(Vector3D &zmiana);
	virtual std::string typKlasy() { return "Door"; };
};

#endif
