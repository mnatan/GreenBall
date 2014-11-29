#ifndef BOX_CPP
#define BOX_CPP

#include "Box.h"

Box::Box(Vector3D pos_) : moved(pos_, TEX_BOX)
{
	animating = false;
	endTime = current_time;
}

void Box::playerEnters(Vector3D zmiana)
{
	setAnimation(pos,
	             pos + zmiana,
	             ANIM_PLAYER_TIME
	            );
}

bool Box::canEnter(Vector3D &zmiana)
{
	if (MapRead(pos + zmiana).canEnter(zmiana)) return true;
	else return false;

}

bool Box::drawIt()
{
	DrawCubeTexture(                // Czo sie dzieje :<
	    pos + Vector3D(0, 0, 0.5),
	    1.0f,
	    TEX
	);
	return true;
}

#endif
