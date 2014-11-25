#ifndef TRIGGER
#define TRIGGER

#include "Responser.cpp"
#include "../define.h"

class Trigger
{
	public:
	Responser *target;
	bool triggered;

	bool ActivateTarget();
	bool DeactivateTarget();
};

#endif
