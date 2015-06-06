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

// Sprawdza czy na tym miejscu nie ma elementów uniemożliwiających wejście
bool MapChunk::canEnter(Map& map, Vector3D& zmiana)
{
    bool can = true;

    for (auto x : zawartosc)
        can &= x->canEnter(map, zmiana);

    return can;
}

// Sprawdza czy to miejsce może utrzymać obiekt
bool MapChunk::canFall()
{
    bool can = false;

    for (auto x : zawartosc)
        can |= x->canFall();

    return can;
}

// Rysuje cały pion warstw mapy
bool MapChunk::drawIt()
{
    for (auto x : zawartosc)
        x->drawIt(); // TODO not pointer?

    return true;
}

// Informujemy wszystkie obiekty na polu że wchodzi tam gracz
bool MapChunk::playerEnters(Map& map, Vector3D& delta)
{
    for (auto x : zawartosc)
        x->playerEnters(map, delta);

    return true;
}

void MapChunk::print_zawartosc()
{
    std::cout << "MapChunk :" << std::endl;

    for (auto x : zawartosc)
        std::cout << "- " << x->typKlasy() << std::endl;
}

#endif
