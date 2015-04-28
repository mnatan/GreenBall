#ifndef GEM
#define GEM

#include "rotated.cpp"

class Gem : public rotated
{
	public:
	Gem(Vector3D pos_, Vector3D vector, float angle_, float acc) : rotated(pos_, vector, angle_, acc) {}
	bool drawIt();
	void playerEnters(Map & map, Vector3D &zmiana);
};

#endif
