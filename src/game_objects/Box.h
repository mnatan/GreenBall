#ifndef BOX
#define BOX

#include "moved.h"

class Box: public moved
{
	public:
	Box(Vector3D pos_);
	bool drawIt();
	bool canEnter(Vector3D &delta);
	void playerEnters(Vector3D &delta);
	virtual std::string typKlasy() { return "Box"; };
};

#endif
