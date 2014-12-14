#ifndef BOX_CPP
#define BOX_CPP

#include "Box.h"
#include <algorithm>

Box::Box(Vector3D pos_) : moved(pos_, TEX_BOX)
{
	animating = false;
	endTime = current_time;
}

void Box::playerEnters(Vector3D &zmiana)
{
	MapRead(pos + zmiana).playerEnters(zmiana);

	MapRead(pos + zmiana).zawartosc.push_back(this);
	auto iter = std::find(
	                MapRead(pos).zawartosc.begin(),
	                MapRead(pos).zawartosc.end(),
	                (Box*)this);
	if (iter != MapRead(pos).zawartosc.end())
	{
		MapRead(pos).zawartosc.erase(iter);
	}

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
	UpdateAnimation();
	DrawCubeTexture(                // Czo sie dzieje :<
	    pos + Vector3D(0, 0, -0.5),
	    1.0f,
	    TEX
	);
	return true;
}

#endif
