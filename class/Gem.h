#ifndef GEM
#define GEM

#include "rotated.cpp"
#include "../define.h"

class Gem : public rotated {
	public:
		Gem(Vector3D pos_, Vector3D vector, float angle_, float acc) : rotated(pos_, vector, angle_, acc){}
		bool drawIt();
};

#endif
