#ifndef SWITCH
#define SWITCH

#include "trigger.cpp"
#include "game_obj.cpp"

class Switch: public game_obj, public Trigger
{
	public:
	Switch(Vector3D pos_, Responser target_);
};

#endif
