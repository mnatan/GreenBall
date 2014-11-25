#ifndef BOX_CPP
#define BOX_CPP

#include "Box.h"

Box::Box(Vector3D pos_) : moved(pos_, TEX_BOX){
	animating = false;
	endTime = current_time;
}

#endif
