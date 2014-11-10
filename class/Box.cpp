#ifndef BOX_CPP
#define BOX_CPP

#include "Box.h"

Box::Box(Vector3D pos_){
	active = false;
	endTime = current_time;
	pos = pos_;
}

#endif
