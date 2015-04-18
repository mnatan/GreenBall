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
	zawartosc = std::vector<game_obj*>();
}

bool MapChunk::canEnter(Vector3D &zmiana)       // Sprawdza czy na tym miejscu nie ma elementów uniemożliwiających wejście
{
	bool can = true;
	std::vector<game_obj*>::iterator end = zawartosc.end();
	for (std::vector<game_obj*>::iterator iter = zawartosc.begin(); iter != end; ++iter)
	{
		can &= (*iter)->canEnter(zmiana);
	}
	return can;
}

bool MapChunk::canFall()        // Sprawdza czy to miejsce może utrzymać obiekt
{
	bool can = false;
	std::vector<game_obj*>::iterator end = zawartosc.end();
	for (std::vector<game_obj*>::iterator iter = zawartosc.begin(); iter != end; ++iter)
	{
		can |= (*iter)->canFall();
	}
	return can;
}

bool MapChunk::drawIt()         // Rysuje cały pion warstw mapy
{
	std::vector<game_obj*>::iterator end = zawartosc.end();
	for (std::vector<game_obj*>::iterator iter = zawartosc.begin(); iter != end; ++iter)
	{
		//std::cout << (*iter)->typKlasy() << std::endl;
		(*iter)->drawIt();
	}
	return true;
}

bool MapChunk::playerEnters(Vector3D &zmiana)   // Informujemy wszystkie obiekty na polu że wchodzi tam gracz
{
	auto end = zawartosc.end();
	for (auto iter = zawartosc.begin(); iter != end; ++iter)
	{
		//std::cout << (*iter)->typKlasy() << std::endl;
		(*iter)->playerEnters(zmiana);
	}
	return true;
}

void MapChunk::print_zawartosc()
{
	auto end = zawartosc.end();
	std::cout << "MapChunk :" << std::endl;
	for (auto iter = zawartosc.begin(); iter != end; ++iter)
	{
		std::cout << "- " << (*iter)->typKlasy() << std::endl;
	}
}

#endif
