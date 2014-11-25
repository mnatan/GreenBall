#ifndef MAPCHUNK_CPP
#define MAPCHUNK_CPP

#include "MapChunk.h"

MapChunk::MapChunk()
{
	zawartosc = std::vector<animated*>();
	statyczny = new EmptySpace();
}

#endif
