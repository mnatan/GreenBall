#ifndef SWITCH_CPP
#define SWITCH_CPP

#include "Switch.h"

Switch::Switch(Vector3D pos_, Responder target_) : animated(pos_, TEX_SWITCH)
{
	target = &target_;
	animating = false;
}
bool Switch::drawIt()
{
	//TODO FIXME WHAT HAPPENS
    GreenEngine::DrawQuadTexture(
		pos + Vector3D(0, 0, 0.1f),
		1.0f, 1.0f,
		TEX
	);
	return true;
}

#endif
