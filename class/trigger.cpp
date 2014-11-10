#ifndef TRIGGER_CPP
#define TRIGGER_CPP

#include "trigger.h"

bool Trigger::ActivateTarget()
{
	return target->respondON();
}
bool Trigger::DeactivateTarget()
{
	return target->respondOFF();
}

#endif
