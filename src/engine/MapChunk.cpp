#ifndef MAPCHUNK_CPP
#define MAPCHUNK_CPP

#include "MapChunk.h"
#include "../game_objects/game_obj.cpp"
/*
 *#include "../game_objects/animated.cpp"
 *#include "../game_objects/EmptySpace.cpp"
 *
 *#include "animated.cpp"
 *#include "game_obj.cpp"
 *#include "EmptySpace.cpp"
 */


MapChunk::MapChunk()
{
	zawartosc = std::vector<game_obj>();
}

bool MapChunk::canEnter(Map & map, Vector3D &zmiana)       // Sprawdza czy na tym miejscu nie ma elementów uniemożliwiających wejście
{
	bool can = true;
	auto end = zawartosc.end();
	for (auto iter = zawartosc.begin(); iter != end; ++iter)
	{
		can &= (*iter).canEnter(map, zmiana);
	}
	return can;
}

bool MapChunk::canFall()        // Sprawdza czy to miejsce może utrzymać obiekt
{
	bool can = false;
	auto end = zawartosc.end();
	for (auto iter = zawartosc.begin(); iter != end; ++iter)
	{
		can |= (*iter).canFall();
	}
	return can;
}

bool MapChunk::drawIt()         // Rysuje cały pion warstw mapy
{
	auto end = zawartosc.end();
	for (auto iter = zawartosc.begin(); iter != end; ++iter)
	{
		//std::cout << (*iter)->typKlasy() << std::endl;
		(*iter).drawIt();
	}
	return true;
}

bool MapChunk::playerEnters(Map & map, Vector3D &delta)   // Informujemy wszystkie obiekty na polu że wchodzi tam gracz
{
	auto end = zawartosc.end();
	for (auto iter = zawartosc.begin(); iter != end; ++iter)
	{
		//std::cout << (*iter)->typKlasy() << std::endl;
		(*iter).playerEnters(map, delta);
	}
	return true;
}

void MapChunk::print_zawartosc()
{
	auto end = zawartosc.end();
	std::cout << "MapChunk :" << std::endl;
	for (auto iter = zawartosc.begin(); iter != end; ++iter)
	{
		std::cout << "- " << (*iter).typKlasy() << std::endl;
	}
}

#endif