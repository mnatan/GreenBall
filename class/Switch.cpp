#ifndef SWITCH_CPP
#define SWITCH_CPP

#include "Switch.h"

Switch::Switch(Vector3D pos_, Responser target_){
	pos = pos_;
	target = &target_;
	active = false;
}

#endif
