#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "player.h"

bool Player::drawIt()
{
    //if(animating) animating = UpdateAnimation();

    GreenEngine::DrawQuadTexture(
        pos + Vector3D(0, 0, 0.5f),
        0.8f,
        0.8f,
        TEX_PLAYER
    );
    return true;
}

#endif
