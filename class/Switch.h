#ifndef SWITCH
#define SWITCH

#include "trigger.cpp"
#include "animated.cpp"

class Switch: public animated, public Trigger
{
	public:
	Switch(Vector3D pos_, Responser target_);

	bool drawIt();
};

#endif
