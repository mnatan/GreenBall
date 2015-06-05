#ifndef FLOOR_CPP
# define FLOOR_CPP

# include "Floor.h"

Floor::Floor(Vector3D pos_): game_obj(pos_, TEX_FLOOR), stable(true)
{
}

bool Floor::canEnter(Map& map, Vector3D& zmiana)
{
    return stable;
}

bool Floor::drawIt()
{
    GreenEngine::DrawCubeTexture(
        pos + Vector3D(0, 0, -0.5f),
        1.0f,
        TEX
    );
    return true;
}

#endif
