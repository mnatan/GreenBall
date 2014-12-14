#ifndef MAPCHUNK
#define MAPCHUNK

#include <vector>
#include "animated.cpp"
#include "game_obj.cpp"
#include "EmptySpace.cpp"

class MapChunk
{
	public:
	std::vector<game_obj*> zawartosc;

	MapChunk();
	bool canFall();
	bool drawIt();
	bool canEnter(Vector3D &zmiana);
	bool playerEnters(Vector3D &zmiana);
	void print_zawartosc();
};

#endif
