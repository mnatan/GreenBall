#ifndef MAPCHUNK
#define MAPCHUNK

#include <vector>
#include "animated.cpp"
#include "game_obj.cpp"
#include "EmptySpace.cpp"

class MapChunk
{
	public:
	std::vector<animated*> zawartosc;
	game_obj* statyczny;

	MapChunk();
};

#endif
