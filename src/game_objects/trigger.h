#ifndef TRIGGER
#define TRIGGER

#include "Responder.cpp"

class Trigger
{
	public:
	Responder *target;
	bool triggered;

	bool ActivateTarget();
	bool DeactivateTarget();
};

#endif
