#ifndef MAPCHUNK
#define MAPCHUNK

#include <vector>
#include "../game_objects/game_obj.cpp"
#include "../game_objects/animated.cpp"
#include "../game_objects/EmptySpace.cpp"

class MapChunk
{
	public:
	std::vector<game_obj*> zawartosc;

	MapChunk();
	bool canFall();
	bool drawIt();
	bool canEnter(Map & map, Vector3D &zmiana);
	bool playerEnters(Map & map, Vector3D &zmiana);
	void print_zawartosc();
};

#endif

