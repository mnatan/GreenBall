#ifndef BOX
#define BOX

#include "moved.h"
#include "../define.h"

class Box: public moved
{
	public:
	Box(Vector3D pos_);
	bool drawIt();
	bool canEnter(Vector3D &zmiana);
	void playerEnters(Vector3D &zmiana);
	virtual std::string typKlasy() { return "Box"; };
};

#endif
