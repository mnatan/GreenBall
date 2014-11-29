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
	bool canEnter();
	bool canFall();
	bool drawIt();
};

#endif