#ifndef MAPCHUNK_CPP
#define MAPCHUNK_CPP

#include "MapChunk.h"

MapChunk::MapChunk()
{
	zawartosc = std::vector<game_obj*>();
}

bool MapChunk::canEnter()		// Sprawdza czy na tym miejscu nie ma elementów uniemożliwiających wejście
{
	bool can = true;
	std::vector<game_obj*>::iterator end = zawartosc.end();
	for(std::vector<game_obj*>::iterator iter = zawartosc.begin(); iter != end; ++iter)
	{
		can &= (*iter)->canEnter();
	}
	return can;
}

bool MapChunk::canFall()		// Sprawdza czy to miejsce może utrzymać obiekt
{
	bool can = false;
	std::vector<game_obj*>::iterator end = zawartosc.end();
	for(std::vector<game_obj*>::iterator iter = zawartosc.begin(); iter != end; ++iter)
	{
		can |= (*iter)->isSolid();
	}
	return can;
}
bool MapChunk::drawIt()			// Rysuje cały pion warstw mapy
{
	std::vector<game_obj*>::iterator end = zawartosc.end();
	for(std::vector<game_obj*>::iterator iter = zawartosc.begin(); iter != end; ++iter)
	{
		(*iter)->drawIt();
	}
	return true;
}

#endif
