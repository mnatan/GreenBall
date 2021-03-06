#ifndef BLNKER
# define BLNKER

# include "scaled.h"

class Blinker: public scaled
{
    public:
        float time_dead;
        float time_alive;

        float next_switch;

        enum stan
        {
            visible = 1,
            shrinking = 2,
            hidden = 3,
            growing = 4
        };

        stan state;     // 1-jest, 2-zmiana, 3-nie ma, 4-zmiana

        Blinker(Vector3D pozycja, float time_dead_, float time_alive_);
        bool UpdateAnimation();
        bool drawIt();

        bool canEnter(Vector3D& zmiana);
        bool canFall();
};
#endif
