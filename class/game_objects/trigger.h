#ifndef TRIGGER
#define TRIGGER

#include "Responser.cpp"

class Trigger
{
	public:
	Responser *target;
	bool triggered;

	bool ActivateTarget();
	bool DeactivateTarget();
};

#endif
