#ifndef TRIGGER
#define TRIGGER

#include "Responser.cpp"
#include "../define.h"

class Trigger
{
	public:
	Responser *target;
	bool active;

	bool ActivateTarget();
	bool DeactivateTarget();
};

#endif
