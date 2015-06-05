#ifndef BOX_CPP
# define BOX_CPP

# include "Box.h"
# include "src/engine/Map.cpp"
# include <algorithm>

Box::Box(Vector3D pos_): moved(pos_, TEX_BOX)
{
    animating = false;
    //endTime = current_time; BUG?
}

void Box::playerEnters(Map& map, Vector3D& zmiana)
{
    map.access(pos + zmiana).playerEnters(map, zmiana);

    map.access(pos + zmiana).zawartosc.push_back(this);
    /*
     *auto iter = std::find(
     *                map.access(pos).zawartosc.begin(),
     *                map.access(pos).zawartosc.end(),
     *                (Box*)this);
     *if (iter != map.access(pos).zawartosc.end())
     *{
     *    map.access(pos).zawartosc.erase(iter);
     *}
     */

    //setAnimation(pos, pos + zmiana, ANIM_PLAYER_TIME); TODO
}

bool Box::canEnter(Map& map, Vector3D& zmiana)
{
    if (map.access(pos + zmiana).canEnter(map, zmiana))
        return true;
    else
        return false;

}

bool Box::drawIt()
{
    UpdateAnimation();
    GreenEngine::DrawCubeTexture(   // Czo sie dzieje :<
        pos + Vector3D(0, 0, -0.5),
        1.0f, TEX
    );
    return true;
}

#endif
