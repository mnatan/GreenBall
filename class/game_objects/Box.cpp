#ifndef BOX_CPP
#define BOX_CPP

#include "Box.h"
#include "class/engine/Map.cpp"
#include <algorithm>

Box::Box(Vector3D pos_) : moved(pos_, TEX_BOX)
{
	animating = false;
	endTime = current_time;
}

void Box::playerEnters(Map & map,Vector3D &zmiana)
{
	map.access(pos + zmiana).playerEnters(zmiana);

	map.access(pos + zmiana).zawartosc.push_back(this);
	auto iter = std::find(
	                map.access(pos).zawartosc.begin(),
	                map.access(pos).zawartosc.end(),
	                (Box*)this);
	if (iter != map.access(pos).zawartosc.end())
	{
		map_map.access(pos).zawartosc.erase(iter);
	}

	setAnimation(pos,
	             pos + zmiana,
	             ANIM_PLAYER_TIME
	            );
}

bool Box::canEnter(Vector3D &zmiana)
{
	if (map_map.access(pos + zmiana).canEnter(zmiana)) return true;
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
